#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
      cout << "client :" << inet_ntoa(client_addr->sin_addr) << " say bye!" << endl;
      close(client_fd);
      break;
    }
    buf[read_size] = '\0';
    cout << "client :" << inet_ntoa(client_addr->sin_addr) << " say bye!" << endl;
    write(client_fd,buf,strlen(buf));
  }
  return;
}

void CreatWorker(int client_fd,struct sockaddr_in* client_addr)
{
  pid_t id = fork();
  if(id < 0)
  {
    exit(1);
  }
  else if(id == 0)
  {
    if(fork() == 0)
    {  
       ProcessRequest(client_fd,client_addr);
    }
    exit(0);
  }
  else{
    close(client_fd);
    waitpid(id,NULL,0);
  }
}

int main(int argc,char*argv[])
{
  if(argc != 3)
  {
    Usage();
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));

int fd = socket(AF_INET,SOCK_STREAM,0);
  if(fd < 0)
  {
    return 1;
  }

  int ret = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
  if(ret < 0)
  {
    return 1;
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
    CreatWorker(client_fd,&client_addr);
  }
  return 0;
}
