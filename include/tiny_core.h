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

typedef tiny_int_t tiny_port_t;
typedef tiny_string_t tiny_version_t;
typedef tiny_string_t tiny_uri_t;

class Tiny{
public:
    Tiny(const tiny_string_t &);
    tiny_int_t run();
    ~Tiny();
    Tiny& get(tiny_uri_t, tiny_http_handler_pt, int flag=0);
    void* work(void *);
private:
	static const tiny_version_t version;
    tiny_int_t run(tiny_port_t);
    Sbuf<tiny_socket_t*> socket_queue;
    ThreadPool worker;
	tiny_conf_t config;
    tiny_router_t router_list;
    tiny_int_t http_handler(tiny_socket_t*);
    tiny_int_t response_handler(const tiny_http_request_t&, tiny_http_response_t&);
};

}
