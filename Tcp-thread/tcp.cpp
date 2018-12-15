#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

using namespace std;

void Usage()
{
  cout << "usage: ./server [ip] [port]" << endl;
}

void ProcessRequest(int client_fd,struct sockaddr_int* client_addr)
{
  char buf[1024] = {0};
  for(;;)
  {
    ssize_t read_size = read(client_fd,buf,sizeof(buf));
    if(read_size < 0)
    {
      continue;
    }
    if(read_size == 0)
    {
      cout << "client " << inet_ntoa(client_addr->sin_addr) << "say bye!" << endl;
      close(client_fd);
      break;
    }
    buf[read_size] = '\0';
    cout << "client " << inet_ntoa(client_addr->sin_addr) 
      <<"say: " << buf << endl;
    write(client_fd,buf,strlen(buf));
  }
  return;
}

typedef struct Arg
{
  int fd;
  struct sockaddr_in addr;
}Arg;
