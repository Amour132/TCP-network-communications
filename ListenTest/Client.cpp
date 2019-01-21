#include "Socket.hpp"
int main(int argc, char* argv[]) 
{
  if (argc != 3) 
 {
    cout <<"Usage ./test_client [ip] [port]" << endl;
    return 1;
 }
   Socket sock;
   bool ret = sock.Connect(argv[1], atoi(argv[2]));
 if (ret) 
 {
  cout<<"connect ok"<<endl;
 } 
 else 
 {
    cout <<"connect failed" << endl;;
 }
 while (1) 
 {
    sleep(1);
 }
 return 0;
}
