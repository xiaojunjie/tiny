#include <errno.h>
#include <string.h>
#include "tiny_socket_io.h"
#include "tiny_file.h"
namespace tiny{

static tiny_int_t read_char(tiny_socket_t &socket, tiny_char_t* c, tiny_int_t len=1){
    if(socket.start_p >= socket.end_p){
        int cnt = ::read(socket.fd, socket.buf, BUFSIZE);
        if(cnt < 0){
            if(errno==EAGAIN)
                return 0;
            else
                return TINY_ERROR;
        }else if(cnt==0){ /*EOF*/
            return 0;
        }else{
            /*Reset*/
            socket.start_p = socket.buf;
            socket.end_p = socket.buf+cnt;
        }
    }
    tiny_int_t result = MIN(len, socket.end_p-socket.start_p);
    for(int i=0; i<result; i++,socket.start_p++,c++){
        *c = *socket.start_p;
    }
    return result;
}

tiny_int_t tiny_socket_read(tiny_socket_t& socket, tiny_string_t& str){
    char buf[MAXLINE] = {0};
    tiny_int_t s = read_char(socket, buf, MAXLINE);
    str = tiny_string_t(buf);
    return s;
}

tiny_int_t tiny_socket_readline(tiny_socket_t& socket, tiny_string_t& str){
    char buf[MAXLINE];
    int i;
    for(i=0; i<MAXLINE; i++){
        tiny_int_t s = read_char(socket, &buf[i]);
        if (s == TINY_SUCCESS){
            if(buf[i]=='\n'){
                i++;
                break;
            }
        } else if (s == TINY_OVER){
            if(i==0)
                return TINY_OVER; /*EOF without data*/
            else
                break; /*EOF with data*/
        } else {
            return TINY_ERROR;
        }
    }
    buf[i] = 0;
    str = tiny_string_t(buf);
    return TINY_SUCCESS;
}

tiny_int_t tiny_socket_write(tiny_socket_t& socket, const tiny_string_t& str){
    ssize_t nwritten;
    char* bufp = (char *)str.c_str();
    size_t nleft = str.length();
    while (nleft > 0) {
        if ((nwritten = write(socket.fd, bufp, nleft)) <= 0) {
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
