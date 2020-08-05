#include<stdio.h>
#include<string.h>

#define MAXLINE 1024
char *strremove(char *str, const char *sub);
int parse_uri(char *uri, char *host, char *query, char *port);
int main(int argc,char **argv){

  char str[1024];
  char host[1024];
  char query[1024];
  parse_uri(argv[0], host, query,str);
  
  printf("%s %s %s", host, query, str);
  return 0;


}

int parse_uri(char *uri, char *host, char *query, char *port)
{   
    char *url, *ptr2, info[MAXLINE];
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
        //strcpy(host, info);  
        strcpy(temp, ptr2+1);  //cut the : symbol
        *ptr2 = '\0'; 
        //ptr2 = NULL;
        ptr2 = strchr(temp, '/');  //search for the / symbol
        if(*ptr2 == '/'){
//          strcat(query, "/\0");
            strcat(query, ptr2+1); //put the rest of the uri into 
            *ptr2 = '\0';  
            strcpy(port, temp);         //query
//          port = atoi(info);  //convert string type port to int type 
        }
        strcpy(port, temp);
//      port = atoi(info);
    }
    else{
        strcpy(port, "80");  //if the uri doesn't have a : symbol
        ptr2 = strchr(info, '/');
        if(ptr2){
//      strcat(query, "/\0");
        strcat(query, ptr2+1);
        *ptr2 = '\0';
        }
    }
        
    strcpy(host, info);
    return 1;
}

char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

