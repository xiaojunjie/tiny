#include "tiny_core.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <time.h>
#include <unistd.h>

namespace tiny {

const string Tiny::version = "1.0";

Tiny::Tiny(const string &filename) {
    config = tiny_read_conf(filename);
    logger::init(config.logpath, config.loglevel);
    logger::info << "Tiny Web Server " << version;
    logger::info << config << logger::endl;

    router_list.emplace_back("Init", tiny_http_init_handler, 1);
    // router_list.emplace_back("/assets/", tiny_http_static_handler);
    router_list.emplace_back("/", tiny_http_index_handler, 2);
    socket_queue = new Sbuf<void>();
    worker = new ThreadPool<Tiny>(this);
}

Tiny &Tiny::get(tiny_uri_t str, tiny_http_handler_pt handler, int flag) {
    router_list.emplace_back(str, handler, flag);
    return *this;
}

Tiny::~Tiny() {
    delete worker;
    delete socket_queue;
    logger::info << "Tiny Server Shutdown...";
    logger::destroy();
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
    return TinySocketStream::wait(port, [this](tiny_socket_fd_t listenfd) {
        int len = socket_queue->insert((void *)listenfd);
        // std::cout << "len: " << len << std::endl;
    });
}

void *Tiny::work(void *args) {
    tiny_epoll_t epoll;
    while (1) {
        void *argc = socket_queue->remove();
        tiny_socket_fd_t listenfd = *((tiny_socket_fd_t *)(&argc));
        tiny_socket_t *socket = new tiny_socket_t(listenfd);
        if (epoll.add(socket) == TINY_SUCCESS) {
            logger::debug << "[Tiny] socket_handler: " << socket->fd << " -> "
                          << epoll.efd << " on listenfd  " << listenfd;
        } else {
            delete socket;
            std::cout << "!!!!!!" << std::endl;
            continue;
        }
        while (!epoll.empty()) {
            auto sockets = epoll.wait();
            for (auto &item : sockets) {
                tiny_socket_t *p = (tiny_socket_t *)item.first;
                tiny_int_t s = TINY_ERROR;
                if (item.second == 1)
                    s = http_handler(p);
                if (s != TINY_SUCCESS) {
                    epoll.remove(p);
                    delete p;
                }
            }
        }
    }
    return NULL;
}

tiny_int_t Tiny::http_handler(tiny_socket_t *socket) {
    static std::unordered_map<tiny_socket_t *, tiny_http_task_t> cache;

    if (socket == NULL) return TINY_ERROR;
    tiny_string_t buf;
    tiny_int_t n = tiny_socket_read(*socket, buf);
    if (cache.find(socket) == cache.end()) {
        cache[socket] = {socket, new tiny_http_request_t, http_status_type::init};
    }
    auto & task =  cache[socket];
    //std::cout << "before: " << (int)task.status << std::endl;
    tiny_http_event_t event = {n>0 ? http_event_type::in : http_event_type::close, &buf};
    tiny_int_t s = tiny_http_parse_handler(event, task);
    //std::cout << "after: " << (int)task.status << std::endl;
    if (s != TINY_SUCCESS) {
        logger::debug << "[Tiny] http_handler: parse return " << s
                      << logger::endl;
        delete task.request;
        cache.erase(socket);
    }else if(task.status==http_status_type::body){
        auto request = task.request;
        tiny_http_response_t response;
        response_handler(*request, response);
        *socket << response; 
        delete request;
        cache.erase(socket);
    }else if(task.status==http_status_type::over){
        delete task.request;
        cache.erase(socket);
        return TINY_ERROR;
    }
    return s;
}

tiny_int_t Tiny::response_handler(const tiny_http_request_t &request,
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
