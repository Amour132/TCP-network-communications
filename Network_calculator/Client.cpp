#include "Client.hpp"
#include "comm.hpp"
#include <iostream>

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    std::cout << "Usage./deict_clinet" << std::endl;
    return 1;
  }
  Client _client(argv[1],atoi(argv[2]));
  bool ret = _client.Connect();
  if(!ret)
  {
    return 1;
  }
  Rq _rq;
  Rsp _rsp;
  for(;;)
  {
    std::cout << "请输入x,y" << std::endl;
    std::cin >> _rq.x >> _rq.y;
    std::cout << "请输入操作符" << std::endl;
    std::cin >> _rq.pos;
    write(_client.GetSocket(),&_rq,sizeof(_rq));
    read(_client.GetSocket(),&_rsp,sizeof(_rsp));
    std::cout << "status:" << _rsp.Status << std::endl;
    std::cout << "result:" << _rsp.result << std::endl;
  }

  return 0;
}
