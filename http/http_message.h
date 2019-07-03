#pragma once

#include "include/tiny_base.h"
namespace tiny {

typedef tiny_string_t http_body_t;

struct http_request_header_t {
    tiny_string_t content_length;
    tiny_string_t transfer_encoding;
    tiny_string_t accept;
    tiny_string_t connection;
    tiny_string_t cookie;
    tiny_string_t referer;
    tiny_string_t user_agent;
    tiny_string_t host;
    tiny_string_t cache_control;
    tiny_string_t pragma;
};

struct http_response_header_t {
    tiny_int_t content_length;
    tiny_string_t content_type;
    tiny_string_t server;
    tiny_string_t connection;
    tiny_string_t cookie;
    tiny_string_t location;
    tiny_string_t cache_control;
};

struct tiny_http_request_t {
    tiny_string_t method;
    tiny_string_t uri;
    tiny_string_t protocol;
    http_request_header_t header;
    http_body_t body;
    void *data;
};

struct tiny_http_response_t {
    tiny_string_t protocol;
    tiny_int_t status;
    tiny_string_t message;
    http_response_header_t header;
    http_body_t body;
};
}
