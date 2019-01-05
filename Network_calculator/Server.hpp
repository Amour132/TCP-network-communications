#pragma once 

#include "Socket.hpp"
#include <functional>

typedef std::function<void (const std::string& req,std::string* resp)> Handler;

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
        if(!_sock.Accept(new_sock,&ip,&port))
        {
          continue;
        }
        cout << "Get a new..." << endl;
        for(;;)
        {
          std::string req;
          int ret = new_sock.Recv(&req);
          if(ret < 0)
          {
            cout << "disconnect" << endl;
            new_sock.Close();
            break;
          } 
          std::string resp;
          handler(req,&resp);
          new_sock.Send(resp);
        }
      }
      return true;
    }
  private:
      std::string _ip;
      int _port;
      Socket _sock;
};
