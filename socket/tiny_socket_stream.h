#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <functional>
#include <string.h>
#include "tiny_base.h"

#define LISTENQ 1024
#define MAXLINE 1024
#define BUFSIZE 8192
#define TIMEOUT 10
namespace tiny{
    typedef tiny_int_t tiny_socket_port_t;
    typedef tiny_int_t tiny_socket_fd_t;
    typedef struct sockaddr_in tiny_socket_clientaddr_t;
    typedef std::function<void(tiny_socket_fd_t)> tiny_socket_listen_handler;
    class TinySocketStream{
    public:
        TinySocketStream(tiny_socket_fd_t);
        ~TinySocketStream();
        const tiny_socket_fd_t fd;
        tiny_char_t *buf;
        tiny_char_t *start_p; // start of chars available
        tiny_char_t *end_p;
        tiny_socket_fd_t efd;
        tiny_int_t reply_cnt;
        tiny_string_t readline();
        tiny_int_t write_string(const tiny_string_t&);
        static tiny_int_t wait(tiny_socket_port_t, tiny_socket_listen_handler);
        tiny_string_t get_ip();
    private:
        tiny_socket_clientaddr_t clientaddr;
        void close();
        tiny_socket_fd_t accept(tiny_socket_fd_t); // accept socket from listenfd
        //tiny_int_t receive();
        //tiny_int_t readn(tiny_char_t*, tiny_int_t);
        //tiny_int_t readline(void*, tiny_int_t);
        //tiny_int_t nwrite(const void*, tiny_int_t) const;
    };
    typedef TinySocketStream tiny_socket_t;
}
