#include <errno.h>
#include <string.h>
#include <iostream>
#include "tiny_file.h"
#include "tiny_socket_stream.h"
namespace tiny{
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
    static int open_listenfd(tiny_socket_port_t port){
        int listenfd, optval=1;
        struct sockaddr_in serveraddr;
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
        if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
                throw std::runtime_error(strerror(errno));

        /* Make it a listening socket ready to accept connection requests */
        if (listen(listenfd, LISTENQ) < 0)
            throw std::runtime_error(strerror(errno));
        return listenfd;
    }
    int TinySocketStream::accept(tiny_socket_fd_t listenfd){
        socklen_t addrlen = sizeof(clientaddr);
        static std::mutex mtx;
        mtx.lock();
        int rc = ::accept(listenfd, (struct sockaddr*)&clientaddr, &addrlen);
        mtx.unlock();
        if (rc < 0){
            if(errno!=EAGAIN)
                logger::error << "[socket] accept: " << strerror(errno);
        }else{
            make_socket_non_blocking(rc);
        }
        return rc;
    }
    void TinySocketStream::close(){
        if (::close(fd) < 0){
            throw std::runtime_error(strerror(errno));
        }else{
            logger::debug << "[socket] close fd " << fd ;
        }
    }
    static tiny_int_t bind_noticefd(int efd, tiny_socket_fd_t noticefd){
        struct epoll_event event;
        event.data.fd = noticefd;
        event.events = EPOLLIN; // 水平
        int s = epoll_ctl(efd, EPOLL_CTL_ADD, noticefd, &event);
        if(s==-1){
            logger::error << "[socket] bind_noticefd: " << strerror(errno);
        }
        return s;
    }

    tiny_int_t TinySocketStream::wait(tiny_socket_port_t port, tiny_socket_listen_handler callback){
        int listenfd = open_listenfd(port);
        int efd = epoll_create1(0);
        if( efd<0 || 
            listenfd<0 || 
            //make_socket_non_blocking(listenfd)<0 || 
            bind_noticefd(efd,listenfd)<0 )
            throw std::runtime_error(strerror(errno));
        struct epoll_event event;
        while(1){
            int n = epoll_wait(efd, &event, 1, -1);
            if (event.events & EPOLLIN){
                callback(listenfd);
            }else{
                logger::error << "[socket] wait: unknown event " << event.events;
            }
        }
    }
    TinySocketStream::TinySocketStream(tiny_socket_fd_t listenfd)
        :fd(accept(listenfd)){
            reply_cnt = 0;
            buf = new tiny_char_t[BUFSIZE];
            start_p = buf;
            end_p = buf;
            const static struct timeval tv = {TIMEOUT,0};
            setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    }
    
    TinySocketStream::~TinySocketStream(){
        close();
        delete[] buf;
        //std::cout << "close " << fd << " reply "<< reply_cnt << std::endl;
    }
    
   // tiny_int_t TinySocketStream::receive(){
   //     tiny_int_t n = 0;
   //     for(tiny_char_t* end = buf+BUFSIZE; bufptr<end; bufptr+=n){
   //         n = read(fd, bufptr, end-bufptr);
   //         std::cout << "read " << n << " bytes" << std::endl;
   //         if(n<=0)
   //             break;
   //     }
   //     if(n==0 && bufptr<buf){
   //         logger::debug << "[socket] EOF";
   //     }else if(n == -1){
   //         if(errno == EAGAIN)
   //             ;//reset_oneshot(efd,fd,(void*)this);
   //         else
   //             logger::error << "[socket] read error: " << strerror(errno);
   //     }else{
   //         logger::info << "[socket] read overflow";
   //     }
   //     return bufptr-buf;
   // }
   // tiny_int_t TinySocketStream::readn(tiny_char_t* usrbuf, tiny_int_t n){
   //     if ( cnt <= 0 ) { /* Refill if buf is empty */
   // 		cnt = read(fd, buf, BUFSIZE);
   //         if (cnt < 0) {
   //             if(errno==EAGAIN)
   //                 return 0;
   //             else{
   //                 logger::error << "[Socket] readn: errno " << errno;
   //                 return -1;
   //             }
   //         }else if (cnt == 0){ /* EOF */
   //             return 0;
   //         }else{
   //             bufptr = buf;  /* Reset buffer ptr */
   //         }
   //     }
   //     
   //     /* Copy min(n, cnt) bytes from internal buf to user buf */
   //     tiny_int_t read_cnt = MIN(cnt,n);
   //     memcpy(usrbuf, bufptr, read_cnt);
   //     bufptr += read_cnt;
   //     cnt -= read_cnt;
   //     return read_cnt;
   // }
   // tiny_int_t TinySocketStream::readline(void* usrbuf, tiny_int_t maxlen){
   //     tiny_int_t n, rc;
   //     tiny_char_t c, *bufp = (tiny_char_t *)usrbuf;

   //     for (n = 1; n < maxlen; n++) {
   //         if ((rc = readn(&c, 1)) == 1) {
   //             *bufp++ = c;
   //             if (c == '\n') {
   //                 n++;
   //                 break;
   //             }
   //         } else if (rc == 0) {
   //             if (n == 1)
   //                 return 0;  /* EOF, no data read */
   //             else
   //                 break;  /* EOF, some data was read */
   //         } else {
   //             return -1;  /* Error */  //log
   //         }
   //     }
   //     *bufp = 0;
   //     return n-1;
   // }
   // tiny_string_t TinySocketStream::readline(){
   //     tiny_char_t buf[MAXLINE];
   //     tiny_int_t n = readline(buf,MAXLINE);
   //     if(n<=0)
   //         return "";
   //     tiny_char_t *p = strchr(buf,'\r');
   //     if(p==NULL)
   //         return "";
   //     *p = '\0';
   //     return tiny_string_t(buf);
   // }
   // tiny_int_t TinySocketStream::write_string(const tiny_string_t &str){
   //     return nwrite(str.c_str(), str.length());
   // }
   // tiny_int_t TinySocketStream::nwrite(const void *bufs, tiny_int_t n) const{
   //     tiny_int_t nwritten;
   //     tiny_char_t *bufp = (tiny_char_t *)bufs;
   //     tiny_int_t nleft = n;
   //     while (nleft > 0) {
   //         if ((nwritten = write(fd, bufp, nleft)) <= 0) {
   //             if (errno == EINTR) /* Interrupted by sig handler return */
   //                 nwritten = 0;  /* and call write() again */
   //             else
   //                 return -1;  /* errno set by write() */ //log
   //         }
   //         nleft -= nwritten;
   //         bufp += nwritten;
   //     }
   //     return n;
   // }
    tiny_string_t TinySocketStream::get_ip(){
        return tiny_string_t(inet_ntoa(clientaddr.sin_addr));
    }
}
