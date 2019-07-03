#include "socket/tiny_socket.h"
#include "socket/tiny_event.h"
#include "file/file_logger.h"
#include <ctime>
#include <unistd.h>
#include <thread>

using namespace tiny;
using namespace std;
using namespace logger;
int open_clientfd(char *hostname, int port);
int main(){
    if(fork()==0){
        while(1){
            sleep(3);
            int clientfd = open_clientfd("127.0.0.1", 8888);
            write(clientfd,"sdasdada", 8);
            //sleep(1);
            //write(clientfd,"12345678", 8);
            //char buf[1024];
            //read(clientfd, buf, 1024);
            //std::cout << "client: " << buf << std::endl;
            close(clientfd);
        }
        return 0;
    }
    int listenfd = TinySocket::open_listenfd(8888);
    tiny_event_t acceptor;
    tiny_event_t worker;
    thread t([&](){
        while(1){
            auto sockets = worker.wait();
            for(auto socket: sockets){
                tiny_socket_t *p = (tiny_socket_t*)socket.first;
                {
                    int cnt = p->read();
                    if(cnt==0){
                        worker.remove(p);
                        close(p->get_fd());
                        continue;
                    }
                }
                string buf = p->get_buf();
                std::cout << "receive from " << p->get_fd() << " " << socket.second << " ";
                std::cout << buf.length() << " chars: " << buf << std::endl;
                //p->send(buf);
            }
        }
    });
    t.detach();
    acceptor.add(listenfd);
    while(1){
        acceptor.wait();
        auto socket = new tiny_socket_t(listenfd);
        std::cout << "main: " << socket->get_fd () << std::endl;
        worker.add(socket);
    }
    return 0;
}
