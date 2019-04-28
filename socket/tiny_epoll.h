#pragma once

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "tiny_base.h"
#include "tiny_socket_stream.h"
#define MAXEVENTS 16
namespace tiny{

typedef tiny_int_t tiny_epoll_fd_t;
typedef tiny_int_t(*tiny_epoll_handler_pt)(void*);
typedef std::pair<void*,int> tiny_epoll_event_t;

class TinyEpoll {
public:
    TinyEpoll();
    ~TinyEpoll();
    const tiny_epoll_fd_t efd;
    tiny_int_t add(tiny_socket_t*);
    tiny_int_t remove(tiny_socket_t*);
    tiny_int_t modify(tiny_socket_t*);
    std::vector<tiny_epoll_event_t> wait(int);
    tiny_unsigned_t size(){return count;}
    tiny_bool_t empty(){return count==0;}
private:
    tiny_unsigned_t count;
    struct epoll_event *events;
    tiny_epoll_handler_pt event_handler;
};
typedef TinyEpoll tiny_epoll_t;
}
