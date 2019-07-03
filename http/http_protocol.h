#pragma once

#include "include/tiny_base.h"
#include "http_message.h"
#include <sstream>
#include <memory>
#define TINY_HTTP_OK 200
#define TINY_HTTP_NOTFOUND 404
#define TINY_HTTP_REDIRECT 307
namespace tiny{
    std::ostream &operator<<(std::ostream &, tiny_http_request_t const &);
    std::ostream &operator<<(std::ostream &, tiny_http_response_t const &);
    std::ostream &operator<<(std::ostream &, http_request_header_t const &);
    std::ostream &operator<<(std::ostream &, http_response_header_t const &);
    tiny_http_response_t &operator<<(tiny_http_response_t &, const tiny_string_t &);
    tiny_int_t http_parse(tiny_http_request_t &, const tiny_string_t &);
    tiny_int_t http_parse_request_line(const tiny_string_t &, tiny_http_request_t &);
    tiny_int_t http_parse_header(const tiny_string_t &, tiny_http_request_t &);
    tiny_int_t http_parse_body(const tiny_string_t &, tiny_http_request_t &);
    tiny_int_t http_connection_handler(const tiny_http_request_t &, tiny_http_response_t &);
    tiny_int_t http_version_handler(tiny_http_response_t &);
    tiny_int_t http_status_handler(tiny_http_response_t &, tiny_int_t);
    tiny_int_t http_cookie_handler(const tiny_http_request_t &, tiny_http_response_t &);
    tiny_bool_t http_connnecton_status(const tiny_http_response_t &);
    tiny_bool_t http_request_status(const tiny_http_request_t &);
}
