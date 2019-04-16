#pragma once
#include "tiny_base.h"
#include "tiny_http.h"
namespace tiny{

tiny_int_t tiny_http_parse_handler(const tiny_string_t&, tiny_http_request_t&);
typedef tiny_int_t(*tiny_handler_pt)(const struct epoll_event&);

typedef tiny_int_t(*tiny_http_handler_pt)(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_static_handler(const tiny_http_request_t&, tiny_http_response_t&);
tiny_int_t tiny_http_base_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_notfound_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_init_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_index_handler(const tiny_http_request_t &, tiny_http_response_t &);
tiny_int_t tiny_http_file_handler(const std::string &, tiny_http_response_t &);

}
