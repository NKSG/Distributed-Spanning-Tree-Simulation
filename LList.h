/*
 * LinkedList.h
 *
 *  Created on: Dec 31, 2011
 *      Author: dustevil
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "DSTS.h"
/*****************************************************************************
 *                                                                            *
 *  Define a structure for linked list elements.                              *
 *                                                                            *
 *****************************************************************************/
typedef struct ListElmt_ {
	struct Message_ *data;
	struct ListElmt_ *next;
} ListElmt;
/*****************************************************************************
 *                                                                            *
 *  Define a structure for linked lists.                                      *
 *                                                                            *
 *****************************************************************************/
typedef struct List_ {
	int size;
	ListElmt *head;
	ListElmt *tail;
} List;
/*****************************************************************************
 *                                                                            *
 *  --------------------------- Public Interface ---------------------------  *
 *                                                                            *
 *****************************************************************************/
void list_init(List *list);
int list_insert(List *list, const struct Message_ *data);
int list_remove_first(List *list, struct Message_ **data);
#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

#endif /* LINKEDLIST_H_ */
