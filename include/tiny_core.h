#pragma once

#include <initializer_list>
#include <memory>
#include "tiny_base.h"
#include "route/tiny_handler.h"
#include "file/file_logger.h"
#include "file/tiny_file_io.h"
#include "socket/tiny_socket.h"
#include "socket/tiny_event.h"
#include "os/tiny_threadpool.h"
#include "http/http_message.h"
#include "http/http_protocol.h"
#include "route/tiny_router.h"
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
    //Sbuf<tiny_socket_t*> socket_queue;
    std::vector<tiny_event_t*> loop;
    std::mutex loop_mutex;
    ThreadPool worker;
	tiny_conf_t config;
    tiny_router_t router_list;
    tiny_int_t http_server(tiny_socket_t*);
    tiny_int_t request_handler(const tiny_http_request_t&, tiny_http_response_t&);
};

}
