#pragma once 

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
using std::cout;
using std::endl; 

class Socket
{
  public:
    Socket(const std::string& ip,int port):_ip(ip),_port(port)
    {
      _sock = socket(AF_INET,SOCK_STREAM,0);
      if(_sock < 0)
      {
        std::cerr << "socket error" << std::endl;
      }
    }

    bool Bind() 
    {
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(_ip.c_str());
      addr.sin_port = htons(_port);
      int ret = bind(_sock,(sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        std::cerr << "Bind error" << std::endl;
        return false;
      }
      return true;
    }

    bool Listen(int num)
    {
      int ret = listen(_sock,num); 
      if(ret < 0)
      {
        std::cerr << "listen error" << std::endl;
        return false;
      }
      return true;
    }

    bool Accept(Socket* peer)
    {
      sockaddr_in peer_addr;
      socklen_t len = sizeof(peer_addr);
      int new_sock = accept(_sock,(sockaddr*)&peer_addr,&len);
      if(new_sock < 0){
        std::cerr << "accept error" << std::endl;
        return false;
      }
      peer->_sock = new_sock;
      return true;
    }

    bool Recv(std::string* buf)
    {
      buf->clear();
      char tmp[1024] = {0};
      ssize_t read_size = recv(_sock,tmp,sizeof(tmp),0)
      if(read_size < 0)
      {
        std::cerr << "recv error" << std::endl;
        return false;
      }
      else if(read_size == 0)
      {
        return false;
      }
      else 
      {
        buf->assign(tmp,read_size);
        return true;
      }
    }

    bool Send(const std::string& buf)
    {
      ssize_t write_size = send(_sock,buf.data(),buf.size(),0);
      if(write_size < 0)
      {
        std::cerr << "send error" << std::endl;
        return false;
      }
      return true;
    }

    bool Connect()
    {
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(_ip.c_str());
      addr.sin_port = htons(_port);
      int ret = connect(_sock,(sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        std::cerr << "connect error" << std::endl;
        return false;
      }
      return true;
    }
    
    void Close()
    {
      close(_sock);
    }
    ~Socket()
    {
      Close();
    }
  private:
    std::string _ip;
    int _port;
    int _sock;
};
