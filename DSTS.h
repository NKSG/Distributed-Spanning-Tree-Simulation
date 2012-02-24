/*
 * DSTS.h
 *
 *  Created on: Dec 23, 2011
 *      Author: dustevil
 */

#ifndef DSTS_H_
#define DSTS_H_

#include <stdbool.h>
#include "LList.h"

// The constants that will be used throughout the program and the header.

#define MOTE_COUNT 10
#define MAX_COVERED_MOTE_COUNT 5
#define MAX_MESSAGE_LENGHT 8
#define QUEUE_LENGHT 10

typedef enum {
	OFF = 0, IDLE, SELECTED
} MoteState;

// Messages type as enumerated type.
// The purpose of the Parent message is explained.
// The Alarm message notifies the mote to broadcast a Parent message if it is in SELECTED state.

typedef enum {
	PARENT = 0, ALARM
} MessageType;

// Content list of the broadcasted messages.

typedef struct Message_ {
	MessageType mtype;
	struct Mote_ *sender;
} Message;

// The abstraction of a wireless sensor node (mote).

typedef struct Mote_ {
	int ID;
	MoteState state;
	struct List_ *message_queue;
	struct Mote_ *coverageArea[MAX_COVERED_MOTE_COUNT];
	int coveredMoteCount;
} Mote;

// To satisfy the simulation requirements the program need to pass a parameter when it calls the
// proper event handler as looping through mote procedure.
// This is the contents of that parameter.

typedef struct Eventhandler_Param_ {
	Mote *motePtr;
	Mote *parent;
	struct Message_ *parentMessage;
} EventHandler_Param;

typedef void (*EventHandler)(EventHandler_Param *param);

// Not yet used, reserved for future uses

typedef struct Graph_ {
	int vcount;
	int ecount;
	Mote *adjList[MOTE_COUNT];
} Graph;

// The interface of the header.

void initializeNetwork();
Mote *getMote(const int moteID);

#endif /* DSTS_H_ */
