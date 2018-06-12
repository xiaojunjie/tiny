#include "file.h"
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
using namespace std;
class A{
public:
    int a;
};
typedef enum {Debug, Info, Warm, Error, Fatal} Level;
struct B{
   int b;
   A* operator<<(int);
};
        A* B::operator<<(int x){
            this->b = x;
            return new A;
        }
void print(int i){
   logger::info << i << i << i << logger::endl;
   //logger::debug << logger::endl;
}
int main(){
   logger::init("./",logger::Debug);
   for(int i=0;i<1000;i++){
       thread t(&print,i);
       t.detach();
   }
   //logger::debug << 122 << 0 << logger::endl;
   //usleep(100000);
   logger::destroy();
   //shared_ptr<A> p = shared_ptr<A>(new A());
   //p->print();
   //while(1);
   return 1;
}
