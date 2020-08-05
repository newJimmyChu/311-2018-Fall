/* Yanling Wang Fall 2018 */
#include <stdio.h>
#include <stdlib.h>
#include "doublylinkedlist.h"

void insertHead(doublyLinkedList *listPtr, int value) {
  listNode *head = malloc(sizeof(listNode *));   //allocate the new head node
  if(listPtr->head == NULL){
    head->value = value;
    head->prev = NULL;
    head->next = NULL;  //insert the new head node to an empty list
    listPtr->head = head;
    listPtr->tail = head;
    return;
  }
  head->value = value;
  head->prev = NULL;
  head->next = listPtr->head;
  listPtr->head->prev = head; //make head->prev to NULL
  listPtr->head = head;
  return;
}

int removeTail(doublyLinkedList *listPtr) {	
  if(listPtr!= NULL){
    listNode *temp = listPtr->tail;  //make a temp node points to tail
    if(listPtr->tail == listPtr->head){  //if the tail is head
      int value = temp->value;    //make head and tail both to NULL
      listPtr->tail = NULL;
      listPtr->head = NULL;
      free(temp);   //free the tail node
      return value;
    }
    listPtr->tail = temp->prev;   //make list->tail to its prev node
    listPtr->tail->next = NULL;   //free the last node  
    int value = temp->value;  
    free(temp);  
    return value;
  }
  else {
    return 0;
  }
} 

void showHead(doublyLinkedList *listPtr) {
  listNode *head = listPtr->head;
  printf("list head: ");
  while (head != NULL) {
    printf("%d ", head->value);
    head = head->next;
  } 
  printf("nil\n");
  return;
} 
void showTail(doublyLinkedList *listPtr) {
  listNode *tail = listPtr->tail;
  printf("list tail: ");
  while (tail != NULL) {
    printf("%d ", tail->value);
    tail = tail->prev;
  } 
  printf("nil\n");
  return;
} 
void freeList(doublyLinkedList *listPtr) { 
  listNode *head = listPtr->head;
  listNode *temp;
  while(head != NULL) {
    temp = head->next;
    free(head);
    head = temp;
  }
  listPtr->head = NULL;
  listPtr->tail = NULL;
  return;
}

