/*
 * Simulation.c
 *
 *  Created on: Dec 26, 2011
 *      Author: dustevil
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <error.h>
#include "DSTS.h"
#include "LList.h"

// The constants

#define STATE_COUNT 3
#define INPUT_COUNT 2
#define DISPATCH_INTERVAL 5
#define WAKEUP_INTERVAL 10

// The macros

#define simulationDoneMessage() printf("The simulation is successfully completed.\n")
#define initParam(A, B, C) param.motePtr = A; \
		param.parent = B; \
		param.parentMessage = C

// The parameter that will passed to alarming thread from a thread which abstractly represents a mote

typedef struct alarm_t_ {
	int mote_id;
	pthread_t *timer_interupt;
} alarm_t;

// The variables required for the simulation

static EventHandler communicationProtocol[STATE_COUNT][INPUT_COUNT];
static pthread_t threads[MOTE_COUNT];
static int dispatchedList[MOTE_COUNT];
static int dispatchedListEnd = 0;
static sem_t thread_interupts[MOTE_COUNT];
static pthread_mutex_t queue_locks[MOTE_COUNT], lock1, lock2, printout_lock;

// The main programming interface

static void simulation();
static void	initCommunicationProtocol();
static void dispatchRoot();
static void initSemaphores();
static _Bool isNotInList(const int listEnd, const int list[], const int element);
static void thread_dispatch();
static void moteProcedure(void *arg);
/*TODO Alarm bir mesaj olarak mesaj kuyruguna eklenecek ve de while ile sonsuz dongude calisacak her seferinde bir thread detach etmeye gerek yok. */
static void recursiveAlarm();
static void nope(const EventHandler_Param *arg);
static void broadcastParentMessage(const EventHandler_Param *arg);
static void processParentMessage(EventHandler_Param *arg);

int main(int argc, char *argv[])
{
	initializeNetwork();
	simulation();
	simulationDoneMessage();
	return 0;
}

void simulation()
{
	// Loading the FSM representing the communication protocol. Actually this was fairly simple, however gave me the flavor of distributed systems and communication protocols
	initCommunicationProtocol();
	dispatchRoot(); // Detaching the thread representing the root mote
	initSemaphores(); // Detaching the thread representing the root mote

	// Setting the alarm to dispatch a thread according to given interval period
	signal(SIGALRM, thread_dispatch);
	alarm(DISPATCH_INTERVAL);

	// Somewhat blocking the main execution thread, 'cause the dispatched thread representing a mote will never return
	pthread_join(threads[0], NULL);
}

void initCommunicationProtocol() {
	int ctr;
	for(ctr = 0; ctr < INPUT_COUNT; ctr ++)
		communicationProtocol[OFF][ctr] = nope;
	communicationProtocol[IDLE][0] = processParentMessage;
	communicationProtocol[IDLE][1] = nope;
	communicationProtocol[SELECTED][0] = nope;
	communicationProtocol[SELECTED][1] = broadcastParentMessage;
}

void dispatchRoot() {
	pthread_mutex_unlock(&lock1);
	getMote(0)->state = SELECTED;
	pthread_create(&threads[0], NULL, moteProcedure, (void*)getMote(0));
	dispatchedList[dispatchedListEnd++] = 0;
	pthread_mutex_unlock(&lock1);
	printf("Thread 0 is dispatched.\n");
}



void initSemaphores() {
	int ctr;

	for(ctr = 0; ctr < MOTE_COUNT; ctr++) {
		sem_init(&thread_interupts[ctr], 0, 0);
	}
}

void thread_dispatch()
{
	int index;
	_Bool toBreak = false;


	if(dispatchedListEnd < MOTE_COUNT) {
		srand(time(NULL));
		while(!toBreak)
		{
			index = (rand() % (MOTE_COUNT-1)) + 1;
			toBreak = isNotInList(dispatchedListEnd, dispatchedList, index);
		}
		toBreak = false;
		pthread_mutex_lock(&lock1);
		getMote(index)->state = IDLE;
		dispatchedList[dispatchedListEnd++] = index;
		pthread_create(&threads[index], NULL, moteProcedure, (void*)getMote(index));
		pthread_mutex_unlock(&lock1);
		printf("Thread %d is dispatched.\n", index);
		signal(SIGALRM, thread_dispatch);
		alarm(DISPATCH_INTERVAL);
	}

}

// Here is the core of the whole program which represents the procedure a mote will follow during its life span.

void moteProcedure(void *rawMotePtr)
{
	//Variables related to simulation requirements
	Mote *motePtr = (Mote *)rawMotePtr;
	const int mote_id = motePtr->ID;
	pthread_t timer_interupt;
	//Variables related to the communication protocol
	Mote parent;
	const Message broadcastingParentMessage = { PARENT, motePtr };
	Message *nextMessage;
	EventHandler_Param param;

	pthread_create(&timer_interupt, NULL, recursiveAlarm, (void*)motePtr);

	while (true) {
		sem_wait(&thread_interupts[mote_id]);
		if(list_size(motePtr->message_queue) > 0) {
			pthread_mutex_lock(&queue_locks[mote_id]);
			list_remove_first(motePtr->message_queue, &nextMessage);
			pthread_mutex_unlock(&queue_locks[mote_id]);
		}
		switch(nextMessage->mtype) {
		case PARENT :
			initParam(motePtr, &parent, nextMessage);
			break;
		case ALARM :
			initParam(motePtr, &parent, &broadcastingParentMessage);
			break;
		default :
			initParam(NULL, NULL, NULL);
			break;
		}

		communicationProtocol[motePtr->state][nextMessage->mtype](&param);
	}

	return;
}

// A function required for dispatching threads in arbitrary fashion

_Bool isNotInList(const int listEnd, const int list[], const int element)
{
	int i;

	for(i = 0; i < listEnd; i++) {
		if(list[i] == element)
			return false;
	}

	return true;
}

// The procedure of alarm threads which indicates the time overflows to the threads representing motes

void recursiveAlarm(void *rawMotePtr)
{
	const Mote *motePtr = (Mote*)rawMotePtr;
	const int mote_id = motePtr->ID;
	Message alarmMessage = { ALARM , mote_id };

	while(true) {
		sleep(WAKEUP_INTERVAL);
		list_insert(motePtr->message_queue, &alarmMessage);
		sem_post(&thread_interupts[mote_id]);
	}
	return;
}

// Given the input and the state the communication protocol may demand no operation.

static void nope(const EventHandler_Param *arg) {
	return;
}

// Another communication protocol function for processing parent messages

void processParentMessage(EventHandler_Param *arg) {
	Mote *motePtr = arg->motePtr;
	List *listPtr = motePtr->message_queue;
	const Message *parentMessage = arg->parentMessage;

	arg->parent = parentMessage->sender;
	motePtr->state = SELECTED;
	pthread_mutex_lock(&lock2);
	pthread_mutex_unlock(&lock2);
	pthread_mutex_lock(&printout_lock);
	pthread_mutex_unlock(&printout_lock);
}

// The last communication protocol function for broadcasting parent messages

void broadcastParentMessage(const EventHandler_Param *arg) {
	const Mote *motePtr = arg->motePtr;
	Message *parentMessage = arg->parentMessage;
	Mote *receiver;
	int ctr;

	for(ctr = 0; ctr < motePtr->coveredMoteCount; ctr++) {
		receiver  = motePtr->coverageArea[ctr];
		if (receiver->state == OFF)
			continue;
		else {
			pthread_mutex_lock(&queue_locks[receiver->ID]);
			list_insert(receiver->message_queue, parentMessage);
			pthread_mutex_unlock(&queue_locks[receiver->ID]);
			pthread_mutex_lock(&printout_lock);
			printf("The parent message of mote %d is inserted into the message queue of mote %d\n", motePtr->ID, receiver->ID);
			pthread_mutex_unlock(&printout_lock);
			sem_post(&thread_interupts[receiver->ID]);
		}
	}
	pthread_mutex_lock(&printout_lock);
	printf("Mote %d has broadcasted parent message.\n", motePtr->ID);
	pthread_mutex_unlock(&printout_lock);
}
