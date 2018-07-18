#pragma once

#include "file.h"
#include "socket.h"
#include "sbuf.h"
#include "threadpool.h"
#include "http.h"
#include "route.h"
#include <sys/epoll.h>
#include <initializer_list>
#include <memory>
#include <vector>
#include <map>
namespace tiny{

typedef function<HttpResponse*(vector<string>)> make_response_function;
typedef std::map<std::string, make_response_function> RouterTable;

class Tiny{
public:
    Tiny(const string &);
    int run();
    int run(int);
    ~Tiny();
    Tiny& route(const RouterTable &);
    Tiny& route(initializer_list<string>, const make_response_function &);
    Tiny& route(initializer_list<string>);
    Tiny& get(string,const make_response_function &);
    Tiny& post(string,const make_response_function &);
    void* work(void *);
private:
	static const string version;
    Sbuf<SocketStream> *socketQueue;
    ThreadPool<Tiny> *worker;
    Route *router;
	map<const string,const string> ServerConfig;
    int reply(SocketStream *,const HttpResponse *);
    int reply(SocketStream *,shared_ptr<HttpResponse>);
    int parse(SocketStream *,HttpRequest *);
    int notice_handler(const struct epoll_event&, int efd);
    int http_handler(const struct epoll_event&);
    shared_ptr<HttpResponse> route(HttpRequest *);
    static vector<int> noticefds;
    static int add_noticefd(int);
    ssize_t tiny_readline(HttpRequest *request, void* usrbuf, size_t maxlen);
    ssize_t tiny_read(HttpRequest *request, char* usrbuf, size_t maxlen);
    ssize_t tiny_writen(HttpRequest *, const void*, size_t);
    ssize_t tiny_writen(HttpRequest*, string);
    ssize_t tiny_writen(HttpRequest*, ostringstream&);
};

}
