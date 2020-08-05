#include "csapp.h"

int main(int argc, char **argv) 
{
  int n;
  rio_t rio;
  char buf[MAXLINE];
  int fd;
  if(argc == 2){
    fd = Open(argv[1], O_RDONLY, 0);
    while((n = Rio_readn(fd, buf, MAXLINE)) != 0 )
       Rio_writen(STDOUT_FILENO, buf, n);
    return 0;     
  }
  Rio_readinitb(&rio, STDIN_FILENO);
  while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) 
    Rio_writen(STDOUT_FILENO, buf, n);

  return 0;
}



