#pragma once

#include "socket.h"
#include "sbuf.h"
#include "threadpool.h"
#include "http.h"
#include "route.h"
#include "file.h"
#include <initializer_list>
#include <memory>
#include <vector>
#include <map>
#define LOGLEVEL logger::Debug
namespace tiny{

class Tiny{
typedef function<HttpResponse*(vector<string>)> make_response_function;
public:
    Tiny(const string &);
    int run();
    int run(int);
    ~Tiny();
    Tiny& route(string, const make_response_function &);
    Tiny& route(initializer_list<string>, const make_response_function &);
    Tiny& route(initializer_list<string>);
    // Tiny& route(string, HttpResponse *);
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
    shared_ptr<HttpResponse> route(HttpRequest *);
    ssize_t tiny_readline(HttpRequest *request, void* usrbuf, size_t maxlen);
    ssize_t tiny_read(HttpRequest *request, char* usrbuf, size_t maxlen);
    ssize_t tiny_writen(HttpRequest *, const void*, size_t);
    ssize_t tiny_writen(HttpRequest*, string);
    ssize_t tiny_writen(HttpRequest*, ostringstream&);
};

}
