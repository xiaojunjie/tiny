#pragma once
#include "tiny_base.h"
#include "tiny_socket.h"
#include "tiny_http.h"
namespace tiny{

enum class http_event_type{
    in,
    close,
    timeout
};
enum class http_status_type{
    init,
    header,
    body,
    over,
};
enum class http_transition_type{
    init_header,
    header_body,
    header_over,
    header_timeout,
    body_over,
    body_timeout,
    over_init
};

typedef struct {
    http_event_type event;
    void * data;
} tiny_http_event_t;

typedef struct {
    tiny_socket_t *socket;
    tiny_http_request_t *request;
    http_status_type status;
} tiny_http_task_t;

tiny_int_t tiny_http_parse_handler(const tiny_http_event_t &, tiny_http_task_t &);
typedef tiny_int_t(*tiny_handler_pt)(const struct epoll_event&);

typedef tiny_int_t(*tiny_http_handler_pt)(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_static_handler(const tiny_http_request_t&, tiny_http_response_t&);
tiny_int_t tiny_http_base_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_notfound_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_init_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_index_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_file_handler(const std::string &, tiny_http_response_t &);
tiny_int_t tiny_http_json_handler(const std::string &, tiny_http_response_t &);

}
