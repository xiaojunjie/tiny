#include "tiny_core.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <time.h>
#include <unistd.h>

namespace tiny {

const string Tiny::version = "1.0";

Tiny::Tiny(const string &filename) {
    tiny_array_string_t cfgs = tiny_file_readlines(filename);
    for (auto const &item : cfgs) {
        cmatch cm;
        regex e("^(\\S*)\\s*\\=\\s*(\\S*)$");
        regex_match(item.c_str(), cm, e);
        if (cm.size() == 3) {
            config.insert(pair<string, string>(cm[1], cm[2]));
        }
    }
    logger::Level loglevel = (logger::Level)atoi(config["loglevel"].c_str());
    logger::init(config["logpath"], loglevel);
    logger::info << "Tiny Web Server " << version;
    for (auto const &item : config) {
        logger::info << "[Config]" << item.first << " = " << item.second;
    }
    router_list.emplace_back(tiny_http_init_handler);
    router_list.emplace_back("/assets/", tiny_http_static_handler);
    router_list.emplace_back("/", tiny_http_index_handler, 2);
    router_list.emplace_back(tiny_http_notfound_handler);
    router_list.emplace_back(tiny_http_base_handler);
    socket_queue = new Sbuf<void>();
    worker = new ThreadPool<Tiny>(this);
}

Tiny::~Tiny() {
    delete worker;
    delete socket_queue;
    logger::info << "Tiny Server Shutdown...";
    logger::destroy();
}

int Tiny::run() {
    int port = atoi(config["port"].c_str());
    if (port > 0)
        return run(port);
    else
        return -1;
}

int Tiny::run(tiny_port_t port) {
    return TinySocketStream::wait(port, [this](tiny_socket_fd_t listenfd) {
        int len = socket_queue->insert((void *)listenfd);
        std::cout << "len: " << len << std::endl;
    });
}

void *Tiny::work(void *args) {
    tiny_epoll_t epoll;
    while (1) {
        void *argc = socket_queue->remove();
        tiny_socket_fd_t listenfd = *((tiny_socket_fd_t *)(&argc));
        tiny_socket_t *socket = new tiny_socket_t(listenfd);
        if (epoll.add(socket) == TINY_SUCCESS){
            logger::debug << "[Tiny] socket_handler: " << socket->fd << " -> " << epoll.efd
                          << " on listenfd  " << listenfd;
        }else{
            delete socket;
            std::cout << "!!!!!!" << std::endl;
            continue;
        }
        while (!epoll.empty()) {
            std::vector<void*> sockets = epoll.wait();
            for(auto &item: sockets){
                tiny_socket_t *p = (tiny_socket_t*)item;
                http_handler(p);
                epoll.remove(p);
                delete p;
            }
        }
    }
    return NULL;
}

tiny_int_t Tiny::http_handler(tiny_socket_t *socket) {
    if (socket == NULL)
        return TINY_ERROR;
    tiny_http_request_t request;
    tiny_http_response_t response;
    tiny_string_t buf;
    tiny_socket_read(*socket, buf);
    tiny_int_t s = tiny_http_parse_handler(buf, request);
    if (s != TINY_SUCCESS) {
        logger::debug << "[Tiny] http_handler: parse return " << s
                      << logger::endl;
    } else {
        response_handler(request, response);
        // std::cout << "-----------" << std::endl;
        // std::cout << response << std::endl;
        std::ostringstream buf;
        buf << response;
        tiny_socket_write(*socket, buf.str());
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
        }
    }
    return TINY_SUCCESS;
}
}
