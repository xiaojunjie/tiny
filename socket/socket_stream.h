#pragma once

#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdexcept>
#include <functional>
#include <string>

#define LISTENQ 1024
#define MAXLINE 1024
#define BUFSIZE 8192
#define TIMEOUT 60
#define MIN(a,b) (((a)<(b))?(a):(b))
namespace tiny{
    class SocketStream{
    public:
        ~SocketStream();
        SocketStream(int,struct sockaddr_in);
        ssize_t readline(void*, size_t);
        std::string readline();
        ssize_t readn(char*, size_t);
        ssize_t writen(const void*, size_t) const;
        ssize_t writen(const std::string &);
        std::string get_ip();
        static int Wait(int, std::function<void(int,struct sockaddr_in)>);
        const int fd;
    private:
        const struct sockaddr_in clientaddr;
        int cnt;
        char *bufptr;
        char *buf;
        static int Open_listenfd(int);
        static int Accept(int, struct sockaddr *, socklen_t *);
        void Close(int);
    };
}
