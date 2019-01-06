#pragma once 

#include "comm.hpp"
#include "Socket.hpp"
#include <functional>

typedef std::function<void (Rq& req,Rsp* resp)> Handler;

class Server
{
  public:
    Server(const std::string ip,int port):_ip(ip),_port(port) 
    {}

    bool Start(Handler handler)
    {
      _sock.Sock();
      _sock.Bind(_ip,_port);
      _sock.Listen(5);
      for(;;)
      {
        Socket new_sock;
        std::string ip;
        int port = 0;
        if(!_sock.Accept(&new_sock,&ip,&port))
        {
          continue;
        }
        cout << "Get a new..." << endl;
        for(;;)
        {
          Rq req;
          ssize_t ret = read(new_sock.GetSock(),&req,sizeof(req));
          if(ret < 0)
          {
            cout << "disconnect" << endl;
            new_sock.Close();
            break;
          }
          Rsp resp;
          handler(req,&resp);
          write(new_sock.GetSock(),&resp,sizeof(resp));
        }
      }
      return true;
    }

    int GetSocket()
    {
      return _sock.GetSock();
    }

  private:
      std::string _ip;
      int _port;
      Socket _sock;
};
