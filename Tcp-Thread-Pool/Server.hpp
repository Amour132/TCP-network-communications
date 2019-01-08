#pragma once 

#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "ThreadPool.hpp"

class Server
{
  public:

    Server(const int port):_port(port),_sock(-1),_pool(10)
    {}

    void InitServer()
    {
      _sock = socket(AF_INET,SOCK_STREAM,0);
      if(_sock < 0)
      {
        std::cerr << "socket error" << std::endl;
        exit(2);
      }
      struct sockaddr_in local;
      local.sin_family = AF_INET;
      local.sin_port = htons(_port);
      local.sin_addr.s_addr = htonl(INADDR_ANY);

      if(bind(_sock,(sockaddr*)&local,sizeof(local)) < 0)
      {
        std::cerr << "bind error" << std::endl;
        exit(3);
      }

      if(listen(_sock,5) < 0)
      {
        std::cerr << "listen error" << std::endl;
        exit(4);
      }
      signal(SIGCHLD,SIG_IGN);
      _pool.InitPool();
    }

    static void Serv(int sock)
    {
      char buf[1024] = {0};
      for(;;)
      {
        ssize_t r = read(sock,buf,sizeof(buf)-1);
        if(r > 0)
        {
          buf[r] = '\0';
          std::cout << "client#" << buf << std::endl;
          write(sock,buf,sizeof(buf)-1);
        }
        else if(r == 0)
        {
          std::cout << "client quit" << std::endl;
          break;
        }
        else
        {
          std::cerr << "read error" << std::endl;
          break;
        }
      }
    }

    void Start()
    {
      struct sockaddr_in peer;
      for(;;)
      {
        socklen_t len = sizeof(peer);
        int sock = accept(_sock,(sockaddr*)&peer,&len);
        if(sock < 0)
        {
          std::cerr << "accept error" << std::endl;
          continue;
        }
        std::cout << "Get a new..." << std::endl;
        Task t(sock,Server::Serv);
        _pool.PushTask(t);
      }
    }

    ~Server()
    {
      close(_sock);
    }

  private:
    int _sock;
    int _port;
    ThreadPool _pool;
};
