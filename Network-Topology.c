/*
 * Network-Topology.c
 *
 *  Created on: Dec 23, 2011
 *      Author: dustevil
 */
#include <stdlib.h>
#include "DSTS.h"
#include "LList.h"

// The programming interface

static void initializeMoteList();
static void initializeNullMote();


static Mote *moteList[MOTE_COUNT];
static Graph theTopology;
static Mote nullMote;

// Network Initialization. Note that this is handled statically for test pruposes. Besides I didn't want to deal with file operations at the first objective.

void initializeNetwork()
{
	initializeNullMote();
	initializeMoteList();
	theTopology.vcount = MOTE_COUNT;
	theTopology.ecount = 15;
}

void initializeNullMote()
{
	int index;
	nullMote.ID = -1;
	for (index = 0; index < MAX_COVERED_MOTE_COUNT; index++)
		nullMote.coverageArea[index] = NULL;
	nullMote.coveredMoteCount = -1;
}

void initializeMoteList()
{
	int index, y;

	for(index = 0; index < MOTE_COUNT; index++) {
		moteList[index] = (Mote*)malloc(sizeof(Mote));
		moteList[index]->ID = index;
		moteList[index]->message_queue = (List *)malloc(sizeof(List));
		list_init(moteList[index]->message_queue);
		moteList[index]->state = OFF;
		for(y = 0; y < MAX_COVERED_MOTE_COUNT; y++) {
			moteList[index]->coverageArea[y] = &nullMote;
		}
		moteList[index]->coveredMoteCount = 0;
	}

	// Initializing the coverage area of all motes

	moteList[0]->coverageArea[0] = moteList[1];
	moteList[0]->coverageArea[1] = moteList[2];
	moteList[0]->coveredMoteCount = 2;
	moteList[1]->coverageArea[0] = moteList[0];
	moteList[1]->coverageArea[1] = moteList[3];
	moteList[1]->coverageArea[2] = moteList[4];
	moteList[1]->coverageArea[3] = moteList[5];
	moteList[1]->coveredMoteCount = 4;
	moteList[2]->coverageArea[0] = moteList[0];
	moteList[2]->coverageArea[1] = moteList[7];
	moteList[2]->coverageArea[2] = moteList[8];
	moteList[2]->coverageArea[3] = moteList[9];
	moteList[2]->coveredMoteCount = 4;
	moteList[3]->coverageArea[0] = moteList[1];
	moteList[3]->coverageArea[1] = moteList[4];
	moteList[3]->coveredMoteCount = 2;
	moteList[4]->coverageArea[0] = moteList[1];
	moteList[4]->coverageArea[1] = moteList[3];
	moteList[4]->coverageArea[2] = moteList[5];
	moteList[4]->coveredMoteCount = 3;
	moteList[5]->coverageArea[0] = moteList[1];
	moteList[5]->coverageArea[1] = moteList[4];
	moteList[5]->coverageArea[2] = moteList[6];
	moteList[5]->coverageArea[3] = moteList[7];
	moteList[5]->coveredMoteCount = 4;
	moteList[6]->coverageArea[0] = moteList[5];
	moteList[6]->coverageArea[1] = moteList[7];
	moteList[6]->coveredMoteCount = 2;
	moteList[7]->coverageArea[0] = moteList[2];
	moteList[7]->coverageArea[1] = moteList[5];
	moteList[7]->coverageArea[2] = moteList[6];
	moteList[7]->coverageArea[3] = moteList[8];
	moteList[7]->coveredMoteCount = 4;
	moteList[8]->coverageArea[0] = moteList[2];
	moteList[8]->coverageArea[1] = moteList[7];
	moteList[8]->coverageArea[2] = moteList[9];
	moteList[8]->coveredMoteCount = 3;
	moteList[9]->coverageArea[0] = moteList[2];
	moteList[9]->coverageArea[1] = moteList[8];
	moteList[9]->coveredMoteCount = 2;
}

Mote *getMote(const int moteID)
{
	return moteList[moteID];
}
