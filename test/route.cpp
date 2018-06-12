#include <vector>
#include <stdlib.h> 
#include <time.h>
#include <thread>
#include <assert.h>
#include "route.h"
#include <unistd.h>

using namespace std;
using namespace tiny;
int count=0;
int max_r = 100000;
int test_r = 2000000;

int test( Route* p ){
   int s = rand()%max_r;
   HttpRequest* req = new HttpRequest();
   req->SetUri("/"+to_string(s));
   const route_t * route_p  = p->assign(*req);
   if(route_p==NULL)
       cout << "--" << endl;
   vector<string> args;
   HttpResponse *res = route_p->make_response(args);
   if(res==NULL || to_string(s) != res->GetBody())
       cout << "----" << s << endl;
   else
       count++;
   return 1;
}

int test1( Route* p ){
    count++;
}
int main(){
    srand((unsigned)time(NULL));
    Route* p = new Route([](vector<string> args){
        return new HttpResponse(string("defualt")); 
    });
    for(int i=0;i<max_r;i++)
        p->append(
            "/"+to_string(i),
            [i](vector<string> args){
                return new HttpResponse(to_string(i));
            }
        );

    for(int i=0;i<test_r;i++){
        std::thread t(&test1,p);
        t.detach();
    }
    while(count!=test_r){
       cout << count << endl;
       usleep(1000000);
    }
    delete p;
    return 0;
}
