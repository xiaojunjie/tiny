#pragma once
#include "include/tiny_base.h"
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

#define LISTENQ 1024
#define TIMEOUT 10
namespace tiny{
    typedef tiny_int_t tiny_socket_port_t;
    typedef tiny_int_t tiny_socket_fd_t;
    typedef char tiny_socket_buf_t[8192];
    typedef struct sockaddr_in tiny_socket_clientaddr_t;
    class TinySocket{
        public:
            explicit TinySocket(tiny_socket_fd_t);
            ~TinySocket();
            tiny_int_t send(const tiny_string_t &);
            tiny_string_t get_buf() const {return tiny_string_t(inbuf,inbuf+index_inbuf);}
            tiny_bool_t ready() const {return fd>0;}
            tiny_socket_fd_t get_fd() const {return fd;}
            tiny_int_t read();
            tiny_int_t write(const tiny_string_t &);
            tiny_int_t reset();
            static int open_listenfd(tiny_socket_port_t);
        private:
            TinySocket(const TinySocket &);
            TinySocket & operator =(const TinySocket &);
            tiny_socket_fd_t fd;
            tiny_socket_buf_t inbuf;
            tiny_int_t index_inbuf;
    };
    typedef TinySocket tiny_socket_t;
}
