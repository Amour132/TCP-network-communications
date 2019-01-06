#include "Server.hpp"
#include "comm.hpp"

void calculate(Rq &req,Rsp * resp)
{
  
          switch(req.pos)
          {
            case 0:
              resp->result = req.x + req.y;
              break;
            case 1:
              resp->result = req.x - req.y;
              break;
            case 2:
              resp->result = req.x * req.y;
              break;
            case 3:
              if(req.y == 0)
              {
                resp->Status = 1;
                std::cout << "error / 0" << std::endl;
              }
              resp->result = req.x / req.y;
              break;
            default:
              std:: cout << "输入错误" << std::endl;
              break;
          }
}

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    cout << "Usage./diect" << endl;
    return 1;
  }
  Server _server(argv[1],atoi(argv[2]));
  _server.Start(calculate);
  return 0;
}
