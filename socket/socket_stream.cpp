#include "socket_stream.h"
#include "file.h"
#include <errno.h>
#include <string.h>
#include <iostream>
namespace tiny{
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
    int SocketStream::Accept(int s, struct sockaddr *addr, socklen_t *addrlen){
        int rc;
        if ((rc = accept(s, addr, addrlen)) < 0)
                throw std::runtime_error(strerror(errno));
        return rc;
    }
    void SocketStream::Close(int fd){
        int rc;
        if ((rc = close(fd)) < 0)
            throw std::runtime_error(strerror(errno));
        else
            logger::debug << "Close fd " << fd << logger::endl;

    }
    int SocketStream::Wait(int port, std::function<void(int,struct sockaddr_in)> callback){
        struct sockaddr_in clientaddr;
        int listenfd = Open_listenfd(port);
        socklen_t clientlen = sizeof(clientaddr);
        while (1) {
    		int fd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
            if(fd >= 0){
                callback(fd,clientaddr);
            }else{
                logger::error << "connfd: " << fd << logger::endl;
            }
        }
        return 1;
    }
    SocketStream::SocketStream(int a, struct sockaddr_in addr):
        fd(a),
        clientaddr(addr){
            cnt = 0;
            buf = new char[BUFSIZE];
            const static struct timeval tv = {TIMEOUT,0};
            setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    }
    SocketStream::~SocketStream(){
        Close(fd);
        free(buf);
    }
    ssize_t SocketStream::readn(char* usrbuf, size_t n){
        while ( cnt <= 0 ) { /* Refill if buf is empty */
			cnt = read(fd, buf, BUFSIZE);
            if (cnt < 0) {
                if (errno != EINTR) /* Interrupted by sig handler return */
                    return -1;
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
