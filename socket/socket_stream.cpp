#include "socket_stream.h"
#include "file.h"
#include <errno.h>
#include <string.h>
#include <iostream>
namespace tiny{
    int SocketStream::listenfd = -1;
    static int make_socket_non_blocking(int sfd){
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
    int SocketStream::Open_listenfd(int port){
        int listenfd, optval=1;
        struct sockaddr_in serveraddr;
        if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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
        if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
                throw std::runtime_error(strerror(errno));

        /* Make it a listening socket ready to accept connection requests */
        if (listen(listenfd, LISTENQ) < 0)
            throw std::runtime_error(strerror(errno));
        return listenfd;
    }
    int SocketStream::Accept(struct sockaddr *addr, socklen_t *addrlen){
        static std::mutex mtx;
        mtx.lock();
        int rc = accept(listenfd, addr, addrlen);
        mtx.unlock();
        if (rc < 0){
            if(errno!=EAGAIN)
                logger::error << "[socket] accept: " << strerror(errno);
        }else{
            make_socket_non_blocking(rc);
        }
        return rc;
    }
    void SocketStream::Close(int fd){
        if (close(fd) < 0){
            throw std::runtime_error(strerror(errno));
        }else{
            logger::debug << "close fd " << fd ;
        }
    }
    int SocketStream::bind_noticefd(int efd, int noticefd){
        struct epoll_event event;
        event.data.fd = noticefd;
        event.events = EPOLLIN | EPOLLET;
        int s = epoll_ctl(efd, EPOLL_CTL_ADD, noticefd, &event);
        if(s==-1){
            logger::error << "[socket] bind_noticefd: " << strerror(errno);
        }
        return s;
    }
    int SocketStream::add_connect(int efd, int infd, void* ptr){
        struct epoll_event event;
        event.data.ptr = ptr;
        event.events = EPOLLIN | EPOLLET;
        int s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
        if(s==-1){
            logger::error << "[socket] add_connect" << strerror(errno);
        }
        return s;
    }
    int SocketStream::Wait(int port, std::function<void()> callback){
        listenfd = Open_listenfd(port);
        int efd = epoll_create1(0);
        if( efd<0 || 
            listenfd<0 || 
            make_socket_non_blocking(listenfd)<0 || 
            bind_noticefd(efd,listenfd)<0 )
            throw std::runtime_error(strerror(errno));
        struct epoll_event event;
        while(1){
            int n = epoll_wait(efd, &event, 1, -1);
            if (event.events & EPOLLIN){
                callback();
            }
        }
        return 0;
        // struct sockaddr_in clientaddr;
        // socklen_t clientlen = sizeof(clientaddr);
        // while (1) {
        //     int infd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        //     if(infd >= 0){
        //         callback(infd,clientaddr);
        //     }else{
        //         logger::error << "connfd: " << infd << logger::endl;
        //     }
        // }
        // return 0;
    }

    SocketStream::SocketStream(int a, struct sockaddr_in addr):
        fd(a),
        clientaddr(addr){
            reply_cnt = 0;
            cnt = 0;
            buf = new char[BUFSIZE];
            bufptr = buf;
            const static struct timeval tv = {TIMEOUT,0};
            setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    }

    SocketStream::~SocketStream(){
        Close(fd);
        free(buf);
    }
    ssize_t SocketStream::receive(){
        int n = 0;
        for(char* end = buf+BUFSIZE; bufptr<end; bufptr+=n){
            n = read(fd, bufptr, end-bufptr);
            std::cout << "read " << n << " bytes" << std::endl;
            if(n<=0)
                break;
        }
        if(n==0 && bufptr<buf){
            logger::debug << "[socket] EOF";
        }else if(n == -1){
            if(errno == EAGAIN)
                ;//reset_oneshot(efd,fd,(void*)this);
            else
                logger::error << "[socket] read error: " << strerror(errno);
        }else{
            logger::info << "[socket] read overflow";
        }
        return bufptr-buf;
    }
    ssize_t SocketStream::readn(char* usrbuf, size_t n){
        if ( cnt <= 0 ) { /* Refill if buf is empty */
			cnt = read(fd, buf, BUFSIZE);
            if (cnt < 0) {
                if(errno==EAGAIN)
                    return 0;
                else{
                    logger::error << "[Socket] readn: errno " << errno;
                    return -1;
                }
            }else if (cnt == 0){ /* EOF */
                return 0;
            }else{
                bufptr = buf;  /* Reset buffer ptr */
            }
        }
        
        /* Copy min(n, cnt) bytes from internal buf to user buf */
        int read_cnt = MIN(cnt,n);
        memcpy(usrbuf, bufptr, read_cnt);
        bufptr += read_cnt;
        cnt -= read_cnt;
        return read_cnt;
    }
    ssize_t SocketStream::readline(void* usrbuf, size_t maxlen){
        int n, rc;
        char c, *bufp = (char *)usrbuf;

        for (n = 1; n < maxlen; n++) {
            if ((rc = readn(&c, 1)) == 1) {
                *bufp++ = c;
                if (c == '\n') {
                    n++;
                    break;
                }
            } else if (rc == 0) {
                if (n == 1)
                    return 0;  /* EOF, no data read */
                else
                    break;  /* EOF, some data was read */
            } else {
                return -1;  /* Error */  //log
            }
        }
        *bufp = 0;
        return n-1;
    }
    std::string SocketStream::readline(){
        char buf[MAXLINE];
        int n = readline(buf,MAXLINE);
        if(n<=0)
            return "";
        char *p = strchr(buf,'\r');
        if(p==NULL)
            return "";
        *p = '\0';
        return std::string(buf);
    }
    ssize_t SocketStream::writen(const std::string &str){
        return writen(str.c_str(),str.length());
    }
    ssize_t SocketStream::writen(const void *bufs, size_t n) const{
        ssize_t nwritten;
        char *bufp = (char *)bufs;
        size_t nleft = n;
        while (nleft > 0) {
            if ((nwritten = write(fd, bufp, nleft)) <= 0) {
                if (errno == EINTR) /* Interrupted by sig handler return */
                    nwritten = 0;  /* and call write() again */
                else
                    return -1;  /* errno set by write() */ //log
            }
            nleft -= nwritten;
            bufp += nwritten;
        }
        return n;
    }
    std::string SocketStream::get_ip(){
        return std::string(inet_ntoa(clientaddr.sin_addr));
    }
}
