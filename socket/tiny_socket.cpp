#include "socket/tiny_socket.h"
#include "file/file_logger.h"
#include <cstring>

namespace tiny{
    int TinySocket::open_listenfd(tiny_socket_port_t port){
        int listenfd, optval=1;
        ::sockaddr_in serveraddr;
        if ((listenfd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
            throw std::runtime_error(strerror(errno));

        /* Eliminates "Address already in use" error from bind */
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                       (const void *)&optval, sizeof(int)) < 0)
                throw std::runtime_error("Address already in use");

        /* Listenfd will be an endpoint for all requests to port
           on any IP address for this host */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons((unsigned short)port);
        if (bind(listenfd, (::sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
                throw std::runtime_error(strerror(errno));

        /* Make it a listening socket ready to accept connection requests */
        if (listen(listenfd, LISTENQ) < 0)
            throw std::runtime_error(strerror(errno));
        return listenfd;
    }
    static int make_socket_non_blocking(tiny_socket_fd_t sfd){
        int flags, s;
        flags = fcntl(sfd, F_GETFL, 0);
        if(flags == -1){
           return -1;                          
        }
        flags |= O_NONBLOCK;
        s = fcntl(sfd, F_SETFL, flags);
        if(s == -1){
            return -1;                              
        }
        return 0;
    }
    TinySocket::TinySocket(tiny_socket_fd_t listenfd){
        sockaddr_in clientaddr;
        socklen_t addrlen = sizeof(clientaddr);
        fd = ::accept(listenfd, (struct sockaddr*)&clientaddr, &addrlen);
        if(fd<0)
            logger::error << "[socket] accept: " << strerror(errno);
        else
            make_socket_non_blocking(fd);
        const static struct timeval tv = {TIMEOUT,0};
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
        index_inbuf = 0;
    }
    TinySocket::~TinySocket(){
        if(fd<0) return;
        if(::close(fd)<0)
            logger::error << "[socket] close fd " << fd ;
        else
            logger::debug << "[socket] close fd " << fd ;
    }
    tiny_int_t TinySocket::send(const tiny_string_t &str){
        return write(str);
    }
    tiny_int_t TinySocket::reset(){
        index_inbuf = 0;
        return TINY_SUCCESS;
    }
    tiny_int_t TinySocket::read(){
        size_t cnt = ::read(fd, inbuf+index_inbuf, sizeof(inbuf)-index_inbuf);
        if(cnt < 0){
            if(errno==EAGAIN)
                return TINY_OVER;
            else
                return TINY_ERROR;
        }else if(cnt==0){ /*EOF*/
            return TINY_OVER;
        }else{
            index_inbuf += cnt;
        }
        return cnt;
    }
    tiny_int_t TinySocket::write(const tiny_string_t &str){
        size_t nwritten;
        char* bufp = (char *)str.c_str();
        size_t nleft = str.length();
        while (nleft > 0) {
            if ((nwritten = ::write(fd, bufp, nleft)) <= 0) {
                if (errno == EINTR) /* Interrupted by sig handler return */
                    nwritten = 0;  /* and call write() again */
                else if (errno == EAGAIN)
                    nwritten = 0;  /* and call write() again */
                else
                    return TINY_ERROR;  /* errno set by write() */ //log
            }
            nleft -= nwritten;
            bufp += nwritten;
        }
        return TINY_SUCCESS;
    }
}
