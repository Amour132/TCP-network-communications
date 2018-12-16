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

void ProcessRequest(int client_fd,struct sockaddr_in* client_addr)
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
      cout << "client: " << inet_ntoa(client_addr->sin_addr) << "say bye!" << endl;
      close(client_fd);
      break;
    }
    buf[read_size] = '\0';
    cout << "client: " << inet_ntoa(client_addr->sin_addr) <<"say: " << buf << endl;
    write(client_fd,buf,strlen(buf));
  }
  return;
}

typedef struct Arg
{
  int fd;
  struct sockaddr_in addr;
}Arg;

void *CreatWorker(void *ptr)
{
  Arg* arg = (Arg*)ptr;
  ProcessRequest(arg->fd,&(arg->addr));
  free(arg);
  return NULL;
}

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    Usage();
    exit(1);
  }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  int fd = socket(AF_INET,SOCK_STREAM,0);
  if(fd < 0)
  {
    exit(1);
  }

  int ret = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
  if(ret < 0)
  {
    exit(1);
  }

  ret = listen(fd,10);
  if(ret < 0)
  {
    exit(1);
  }

  for(;;)
  {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_fd = accept(fd,(struct sockaddr*)&client_addr,&len);
    if(client_fd < 0)
    {
       continue;
    }
    pthread_t tid = 0;
    Arg* arg = new Arg;
    arg->fd = client_fd;
    arg->addr = client_addr;
    pthread_create(&tid,NULL,CreatWorker,(void*)arg);
    pthread_detach(tid);

  }
  return 0;
}
