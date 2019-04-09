#pragma once

#include "tiny_base.h"
#include "tiny_socket_stream.h"

namespace tiny{
    tiny_int_t tiny_socket_read(tiny_socket_t&, tiny_string_t&);
    tiny_int_t tiny_socket_readline(tiny_socket_t&, tiny_string_t&);
    tiny_int_t tiny_socket_write(tiny_socket_t&, const tiny_string_t&);
}
