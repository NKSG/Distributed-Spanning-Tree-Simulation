/*
 * LinkedList.c
 *
 *  Created on: Dec 31, 2011
 *      Author: dustevil
 */

/*****************************************************************************
 *                                                                            *
 *  -------------------------------- list.c --------------------------------  *
 *                                                                            *
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "LList.h"
/*****************************************************************************
 *                                                                            *
 *  ------------------------------- list_init ------------------------------  *
 *                                                                            *
 *****************************************************************************/
void list_init(List *list) {
	/*****************************************************************************
	 *                                                                            *
	 *  Initialize the list.                                                      *
	 *                                                                            *
	 *****************************************************************************/
	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	return;
}
/*****************************************************************************
 *                                                                            *
 *  ----------------------------- list_ins_next ----------------------------  *
 *                                                                            *
 *****************************************************************************/
int list_insert(List *list, const Message *data) {
	ListElmt *new_element;

	/*****************************************************************************
	 *                                                                            *
	 *  Allocate storage for the element.                                         *
	 *                                                                            *
	 *****************************************************************************/
	if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
		return -1;
	new_element->data = data;
	/*****************************************************************************
	 *                                                                            *
	 *  Insert the element into the tail of the list.                                         *
	 *                                                                            *
	 *****************************************************************************/
	new_element->next = NULL;
	if(list->size == 0) {
		list->head = new_element;
		list->tail = new_element;
	}
	else {
		list->tail->next = new_element;
		list->tail = new_element;
	}

	/*****************************************************************************
	 *                                                                            *
	 *  Adjust the size of the list to account for the inserted element.          *
	 *                                                                            *
	 *****************************************************************************/
	list->size++;
	return 0;
}
/*****************************************************************************
 *                                                                            *
 *  ----------------------------- list_rem_next ----------------------------  *
 *                                                                            *
 *****************************************************************************/
int list_remove_first(List *list, Message **data) {
	ListElmt *old_element;
	/*****************************************************************************
	 *                                                                            *
	 *  Do not allow removal from an empty list.                                  *
	 *                                                                            *
	 *****************************************************************************/
	if (list_size(list) == 0)
		return -1;
	/*****************************************************************************
	 *                                                                            *
	 *  Remove the head of the list.                                         *
	 *                                                                            *
	 *****************************************************************************/

	*data = list->head->data;
	old_element = list->head;
	list->head = list->head->next;
	if (list_size(list) == 1)
		list->tail = NULL;

	/*****************************************************************************
	 *                                                                            *
	 *  Free the storage allocated by the abstract datatype.                      *
	 *                                                                            *
	 *****************************************************************************/
	free(old_element);
	/*****************************************************************************
	 *                                                                            *
	 *  Adjust the size of the list to account for the removed element.           *
	 *                                                                            *
	 *****************************************************************************/
	list->size--;
	return 0;
}
