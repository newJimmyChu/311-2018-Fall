/* Yanling Wang Fall 2018 */
#ifndef LIST_H
#define LIST_H
typedef struct listNode{
    char host[MAXLINE];
    char port[MAXLINE];
    char query[MAXLINE];
    char *memory;
    int memsize;
    struct listNode *next;
    struct listNode *prev;
} listNode;


/*typedef struct listNode {
  int value;
  struct listNode *next;
  struct listNode *prev;
  } listNode;*/

typedef struct doublyLinkedList {
  struct listNode *head;
  struct listNode *tail;
  int block_count;
  int size;
} doublyLinkedList;


void insertHead(doublyLinkedList *listPtr, listNode *head);
int removeTail(doublyLinkedList *listPtr); 
void showHead(doublyLinkedList *listPtr);
void showTail(doublyLinkedList *listPtr);
void freeList(doublyLinkedList *listPtr);
listNode* constructor(char *host, char *port, char *query, char *memory, int size);
void deleteNode(doublyLinkedList *listPtr, listNode *node);
#endif //LIST_H
