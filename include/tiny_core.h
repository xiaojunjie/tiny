#pragma once

#include <initializer_list>
#include <memory>
#include "tiny_base.h"
#include "tiny_handler.h"
#include "tiny_file.h"
#include "tiny_socket.h"
#include "sbuf.h"
#include "threadpool.h"
#include "tiny_http.h"
#include "tiny_router.h"
namespace tiny{

typedef tiny_dict_t tiny_config_t;
typedef tiny_int_t tiny_port_t;
typedef tiny_string_t tiny_version_t;
typedef tiny_string_t tiny_uri_t;

class Tiny{
public:
    Tiny(const tiny_string_t &);
    tiny_int_t run();
    ~Tiny();
    Tiny& route(tiny_uri_t, tiny_http_handler_pt);
    void* work(void *);
private:
	static const tiny_version_t version;
    tiny_int_t run(tiny_port_t);
    Sbuf<void> *socket_queue;
    ThreadPool<Tiny> *worker;
	tiny_config_t config;
    tiny_router_t router_list;
    tiny_int_t http_handler(tiny_socket_t*);
    tiny_int_t response_handler(const tiny_http_request_t&, tiny_http_response_t&);
};

}