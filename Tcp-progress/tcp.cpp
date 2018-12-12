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
      perror("read");
      continue;
    }
    if(read_size == 0)
    {
      cout << "client :" << inet_ntoa(client_addr->sin_addr) << " say bye!" << endl;
      close(client_fd);
      breadk;
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
    perror("forrk");
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

  int fd = socket(AF_INET，SOCK_STREAM,0);
  if(fd < 0)
  {
    perror("socket");
    return 1;
  }
}
