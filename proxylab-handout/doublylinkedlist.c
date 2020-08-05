/* Yanling Wang Fall 2018 */
#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"
#include "doublylinkedlist.h"

/*typedef struct{
    char host[MAXLINE];
    char port[MAXLINE];
    char query[MAXLINE];
    listNode *prev;
    listNode *next;
} listNode;*/ 

listNode* constructor(char *host, char *port, char *query, char *memory, int size){
  //Initialize the new node with web content 
  listNode *node = malloc(sizeof(listNode)+sizeof(memory));
  strcpy(node->host, host);
  strcpy(node->port, port);
  strcpy(node->query, query);
  node->memory = malloc(strlen(memory)+1);
  memcpy(node->memory, memory, strlen(memory)+1);
  node->memsize = size;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void deleteNode(doublyLinkedList *listPtr, listNode *node){
  //Delete the selected node
  if(listPtr->head == NULL || node == NULL) 
    return; 
  if(listPtr->head == node) 
    listPtr->head = node->next; 
  if(node->next != NULL)
    node->next->prev = node->prev; 
  if(node->next == NULL)
    listPtr->tail = node->prev;
  if(node->prev != NULL) 
    node->prev->next = node->next;
  if(node->prev == NULL)
    listPtr->head = node->next;      
  listPtr->size -= node->memsize;
  free(node); 
  return;      
  }



void insertHead(doublyLinkedList *listPtr, listNode *head) {
  //Insert the node to the head
  if(listPtr->head == NULL){
    head->prev = NULL;
    head->next = NULL;  //insert the new head node to an empty list
    listPtr->head = head;
    listPtr->tail = head;
    listPtr->size += head->memsize;
    return;
  }
  head->prev = NULL;
  head->next = listPtr->head;
  listPtr->head->prev = head; //make head->prev to NULL
  listPtr->head = head;
  listPtr->size += head->memsize;
  return;
}

int removeTail(doublyLinkedList *listPtr) {	
  if(listPtr!= NULL){
    listNode *temp = listPtr->tail;  //make a temp node points to tail
    if(listPtr->tail == listPtr->head){  //if the tail is head
      listPtr->tail = NULL;
      listPtr->head = NULL;
      listPtr->size -= temp->memsize;
      free(temp->memory);
      free(temp);   //free the tail node
      return 0;
    }
    listPtr->tail = temp->prev;   //make list->tail to its prev node
    listPtr->tail->next = NULL;   //free the last node  
    listPtr->size -= temp->memsize;
    free(temp->memory);
    free(temp);  
    return 0;
  }
  else {
    return 0;
  }
} 

void showHead(doublyLinkedList *listPtr) {
  listNode *head = listPtr->head;
  printf("list head: ");
  while (head != NULL) {
    printf("%s ", head->host);
    head = head->next;
  } 
  printf("nil\n");
  return;
} 
void showTail(doublyLinkedList *listPtr) {
  listNode *tail = listPtr->tail;
  printf("list tail: ");
  while (tail != NULL) {
    //printf("%d ", tail->value);
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
  listPtr->size = 0;
  return;
}

