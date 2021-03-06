#pragma once
#include "Socket.hpp"


class Client
{
  public:
    Client(const std::string ip,int port):_ip(ip),_port(port)
    {
      _sock.Sock();
    }

    bool Connect()
    {
      return _sock.Connect(_ip,_port);
    }

    bool Recv(std::string* buf)
    {
      return _sock.Recv(buf);
    }

    bool Send(const std::string& buf)
    {
      return _sock.Send(buf);
    }

    ~Client()
    {
      _sock.Close();
    }

    int GetSocket()
    {
      return _sock.GetSock();
    }

  private:
    Socket _sock;
    std::string _ip;
    int _port;
    
};
