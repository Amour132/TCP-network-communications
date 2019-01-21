#include "Socket.hpp"
int main(int argc, char* argv[]) 
{
   if (argc != 3) {
      std::cout <<"Usage ./test_server [ip] [port]" << std::endl;
      return 1;
   }
   Socket sock;
   bool ret = sock.Bind(argv[1],atoi(argv[2]));
   if (!ret) 
   {
      return 1;
   }
   ret = sock.Listen(2);
   if (!ret) 
   {
      return 1;
   }
 // 客户端不进行 accept
   while (1) 
   {
      sleep(1);
   }
   return 0;
}
