#include <errno.h>
#include "tiny_epoll.h"

namespace tiny{
    TinyEpoll::TinyEpoll():efd(epoll_create1(0)){
        if(efd<0)
            throw std::runtime_error(strerror(errno));
        count = 0;
        events = new epoll_event[MAXEVENTS];
    }
    TinyEpoll::~TinyEpoll(){
        delete[] events;
    }

    tiny_int_t TinyEpoll::add(tiny_socket_t* socket){
        if(socket==NULL)
            return TINY_ERROR;
        epoll_event event;
        event.data.ptr = socket;
        event.events = EPOLLIN;
        if(epoll_ctl(efd, EPOLL_CTL_ADD, socket->fd, &event)<0)
            throw std::runtime_error(strerror(errno));
        else
            count ++;
        return TINY_SUCCESS;
    }

    tiny_int_t TinyEpoll::remove(tiny_socket_t* socket){
        if(socket==NULL)
            return TINY_ERROR;
        if (epoll_ctl(efd, EPOLL_CTL_DEL, socket->fd, NULL)<0 )
            throw std::runtime_error(strerror(errno));
        else
            count --;
        return TINY_SUCCESS;
    }

    tiny_int_t TinyEpoll::modify(tiny_socket_t* socket){
        return TINY_SUCCESS;
    }

    std::vector<tiny_epoll_event_t> TinyEpoll::wait(int timeout){
        std::vector<tiny_epoll_event_t> result;
        int n = epoll_wait(efd, events, MAXEVENTS, timeout);
        if(n<0)
            throw std::runtime_error(strerror(errno));
        for (int i = 0; i < n; i++) {
            if (events[i].data.ptr == NULL)
                throw std::runtime_error("tiny epoll null event");
            if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))) {
                throw std::runtime_error("tiny epoll EPOLLERR EPOLLHUP");
            }
            //event_handler(events[i].data.ptr);
            result.emplace_back((void*)events[i].data.ptr, (tiny_int_t)events[i].events);
        }
        return result;
    }
}
