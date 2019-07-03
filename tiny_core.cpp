#include "include/tiny_core.h"
#include <regex>
#include <sstream>
#include <time.h>

namespace tiny {

const string Tiny::version = "1.0";

Tiny::Tiny(const string &filename) {
    config = tiny_read_conf(filename);
    logger::init(config.logpath, config.loglevel);
    logger::info << "Tiny Web Server " << version;
    logger::info << config << logger::endl;

    router_list.emplace_back("Init", tiny_http_init_handler, 1);
    router_list.emplace_back("/", tiny_http_index_handler, 2);
    worker.start( std::bind(&Tiny::work, this, std::placeholders::_1), this );
}

Tiny::~Tiny() {
    logger::info << "Tiny Server Shutdown...";
    logger::destroy();
}

// bind uri to handler
Tiny &Tiny::get(tiny_uri_t str, tiny_http_handler_pt handler, int flag) {
    router_list.emplace_back(str, handler, flag);
    return *this;
}

int Tiny::run() {
    router_list.emplace_back("NoFound", tiny_http_notfound_handler, 1);
    router_list.emplace_back("Base", tiny_http_base_handler, 1);
    if (config.port > 0)
        return run(config.port);
    else
        return -1;
}

int Tiny::run(tiny_port_t port) {
    // main thread: wait for new connection, push tasks to child tasks
    tiny_event_t acceptor;
    int listenfd = TinySocket::open_listenfd(port);
    acceptor.add(listenfd);
    while(loop.empty());
    while(1){
       static int index = 0;
       acceptor.wait();
       tiny_event_t *event = loop[index];
       tiny_socket_t *socket = new tiny_socket_t(listenfd);
       if (socket->ready() && event->add(socket) == TINY_SUCCESS) {
           logger::debug << "[Tiny] socket_handler: " << socket->get_fd() << " -> "
                         << event->efd;
       } else {
           delete socket;
           logger::error << "[Tiny] run: " << event->efd;
       }
       index = (index+1)%loop.size();
    }
    return 0;
}

void* Tiny::work(void *) {
    // child task: accept task child, handle, response
    tiny_event_t event;
    loop_mutex.lock();
    loop.push_back(&event);
    loop_mutex.unlock();
    while (1) {
       auto sockets = event.wait();
       for (auto item : sockets) {
           tiny_socket_t *socket_p = (tiny_socket_t *)item.first;
           // if client close. (EPOLLIN)
           if( socket_p->read() == 0){
               event.remove(socket_p);
               delete socket_p;
               continue;
           }
           tiny_int_t s = http_server(socket_p);
           if (s != TINY_SUCCESS) {
               event.remove(socket_p);
               delete socket_p;
           }
       }
    }
    return NULL;
}

tiny_int_t Tiny::http_server(tiny_socket_t  *socket) {
    assert(socket!=NULL);
    auto buf = socket->get_buf();
    tiny_http_request_t request;
    if(http_parse(request, buf)==TINY_ERROR){
        socket->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        return TINY_ERROR;
    }
    tiny_http_response_t response;
    if(http_request_status(request)){
        request_handler(request, response);
        ostringstream buf;
        buf << response;
        socket->send(buf.str());
        if(!http_connnecton_status(response))
            return TINY_OVER;
        socket->reset();
    }
    return TINY_SUCCESS;
}

tiny_int_t Tiny::request_handler(const tiny_http_request_t &request,
                                  tiny_http_response_t &response) {
    for (auto &item : router_list) {
        if (!item.match(request))
            continue;
        if (item.handler(request, response) == TINY_SUCCESS) {
            // item.count++;
            response.header.server += "[" + item.uri + "]";
        }
    }
    return TINY_SUCCESS;
}
}
