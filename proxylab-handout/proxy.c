#include <stdio.h>
#include "csapp.h"
#include <string.h>
#include "doublylinkedlist.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 16777216 
#define MAX_OBJECT_SIZE 8388608 

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";


void doit(int fd, doublyLinkedList *list);
void clienterror(int fd, char *cause, char *errnum,
		char *shortmsg, char *longmsg);

void read_requesthdrs(rio_t *rp, int client_fd);
int parse_uri(char *uri, char *host, char *query, char *port);
char *strremove(char *str, const char *sub);
int search_cache(doublyLinkedList *listPtr, char *host,
		char *port, char *query, int fd);
int add_cache(doublyLinkedList *listPtr, listNode *node);

int main(int argc, char **argv)
{
    //server stage	
    int listenfd, connfd;
    //char hostname[MAXLINE], port[MAXLINE];
    //char client_port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    doublyLinkedList *list = malloc(sizeof(doublyLinkedList));
    list->head = NULL;
    list->tail = NULL;
    if(argc != 2){
        fprintf(stderr, "usage: %s <port>\n", argv[0]); //need to be implemented
	exit(0);
    }
    
    Signal(SIGPIPE, SIG_IGN);
    listenfd = Open_listenfd(argv[1]);
    while(1){
        clientlen = sizeof(clientaddr);
    	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    	doit(connfd, list);
	Close(connfd);
    }
    //free the whole list
    freeList(list);
    free(list);
    printf("%s", user_agent_hdr);
    return 0;
}


void doit(int fd, doublyLinkedList *list)
{
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char info[MAXLINE];
    char send_buf[MAXLINE];
    char host[MAXLINE], query[MAXLINE];
    char ports[MAXLINE];
    listNode *node;
    int port, client_fd, n;
    rio_t rio, send_rio;
    
    rio_readinitb(&rio, fd); //read a line from the user agent
    if(!rio_readlineb(&rio, buf, MAXLINE))
        return;		//need a error handler
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);    
    if(strcasecmp(method, "GET")){
	clienterror(fd, method, "501", "Not found",
			"Proxy couldn't find this file");
    	return;		//need a error handler
    }
    //handle the HTTP/1.1 & HTTP/1.0 request
    if(!strcmp(version, "HTTP/1.1"))
        strcpy(version, "HTTP/1.0");	    
    if(strcmp(version, "HTTP/1.1")&&strcmp(version, "HTTP/1.0")){
    	clienterror(fd, method, "501", "Not found",
			"Proxy couldn't handle this method");
	return;
    }
    
    //generate the new form of uri
    port = parse_uri(uri, host, query, ports);
    //check if cache hits
    if(search_cache(list, host, ports, query, fd)){
        //printf("%s", list->head->memory);
       return;    
    }
 

    if(port != 1){
        clienterror(fd, uri, "404", "Not found", 
		    "Proxy couldn't parse the request");
        return;
    }
    //Open connection to the end server
    client_fd = Open_clientfd(host, ports);
    rio_readinitb(&send_rio, client_fd);
    sprintf(send_buf, "%s %s %s\r\n", method, query, version);
    rio_writen(client_fd, send_buf, strlen(send_buf));
    sprintf(send_buf, "Host: %s\r\n", host);
    rio_writen(client_fd, send_buf, strlen(send_buf));
    sprintf(send_buf, "%s", user_agent_hdr);
    rio_writen(client_fd, send_buf, strlen(send_buf));
    sprintf(send_buf, "Connection: %s\r\n", "close");
    rio_writen(client_fd, send_buf, strlen(send_buf));
    sprintf(send_buf, "Proxy-Connection: %s\r\n", "close");
    rio_writen(client_fd, send_buf, strlen(send_buf));    
    rio_writen(client_fd, info, strlen(info));
    //forward to the server

    read_requesthdrs(&rio, client_fd);
    char *cache_str = malloc(MAX_OBJECT_SIZE);  
    //forward response
    int sum = 0;
    //reset the cache_str
    strcpy(cache_str, "");
    //Forward the web content to the client and save them
    //to a buffer 
    while((n = rio_readnb(&send_rio, send_buf, MAXLINE)) != 0){
        sum = sum + n;
       	if(sum <= MAX_OBJECT_SIZE){
	    strcat(cache_str, send_buf);
	    printf("Cache saved %d bytes.\n", n);   
        }
        rio_writen(fd, send_buf, n);
    }
   // listNode node1 = malloc(sizeof )
    /*write the cache_str into the cache*/
    if(sum <= MAX_OBJECT_SIZE){
        node = constructor(host, ports, query, cache_str, strlen(cache_str));
        printf("Storing the cache data\n"); 
        add_cache(list, node);
        printf("%s\n", list->tail->host);  
    }
    showHead(list);
    free(cache_str);
    return;
}

void read_requesthdrs(rio_t *rp, int client_fd)
{
    char buf[MAXLINE];

    if(!rio_readlineb(rp, buf, MAXLINE))
        return;
    rio_writen(client_fd, buf, strlen(buf));
    while(strcmp(buf, "\r\n")){
    	rio_readlineb(rp, buf, MAXLINE);
	printf("%s", buf);
    	rio_writen(client_fd, buf, strlen(buf));
    }
    return;
}


int parse_uri(char *uri, char *host, char *query, char *port)
{
    char *url, *ptr2, info[MAXLINE];
    memset(query, 0, strlen(query));
    *query = '/';
    char temp[MAXLINE];
    url = strstr(uri, "http://");  //parse url
    
    if(url){
        strcpy(temp, strremove(uri, "http://"));
        strcpy(info, temp); //cut down the http:// part of the url 
    }
    else{
    	return -1;
    }
    ptr2 = strchr(info, ':');
    if(ptr2){
        strcpy(temp, ptr2+1);  //cut the : symbol
	*ptr2 = '\0';  
	ptr2 = strchr(temp, '/');  //search for the / symbol
	if(*ptr2 == '/'){
	    strcat(query, ptr2+1); //put the rest of the uri into 
	    *ptr2 = '\0';  
	    strcpy(port, temp);    	//query
	}
	strcpy(port, temp);
    }
    else{
        strcpy(port, "80");  //if the uri doesn't have a : symbol
	ptr2 = strchr(info, '/');
	if(ptr2){
	strcat(query, ptr2+1);
	*ptr2 = '\0';
	}
    }	
	
    strcpy(host, info);
    return 1;
}

void clienterror(int fd, char *cause, char *errnum,
	                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF]; 
	  /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);
		 
        /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    sprintf(buf, "%sContent-type: text/html\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}

char *strremove(char *str, const char *sub) 
{
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

//search if the cache alreadly has the object
int search_cache(doublyLinkedList *listPtr, char *host,
		 char *port, char *query, int fd)
{
    int hit = 0;
    if(listPtr->head == NULL){
	return 0;
    }
    listNode *temp = listPtr->head;
    while(temp != NULL){
        if(!strcmp(temp->host, host)&&!strcmp(temp->port, port)
		&&!strcmp(temp->query, query)){
	    //printf("%s", temp->memory);
	    Rio_writen(fd, temp->memory, strlen(temp->memory));
	    //Create a new node, delete the old node and insert
	    //the new one
	    listNode *no = constructor(temp->host, temp->port, temp->query,
			 temp->memory, strlen(temp->memory));
	    deleteNode(listPtr, temp);
	    insertHead(listPtr, no);
	    hit = 1;
	    printf("Cache hit!\n");
  	   // printf("%s\n", no->memory);
	    showHead(listPtr);	
            return hit;    
	}
	temp = temp->next;
    }
    printf("CORRECT!\n");
    return 0;
}

int add_cache(doublyLinkedList *listPtr, listNode *node)
{
    if(listPtr->size <= MAX_CACHE_SIZE){
        insertHead(listPtr, node);
        printf("%s\n", "cache saved");
        printf("%d\n", listPtr->size);
//	printf("%s\n", listPtr->head->memory);
        return 1;
    }   
    else{
	while(listPtr->size > MAX_CACHE_SIZE){
            printf("%d\n", listPtr->size);
	    deleteNode(listPtr, listPtr->tail);
	}
	insertHead(listPtr, node);
	printf("%s", "cache saved");
	return 1;
    }
    return 0;
}










