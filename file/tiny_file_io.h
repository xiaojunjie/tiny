#pragma once

#include <fstream>
#include <sstream>
#include "tiny_base.h"
#include "file_logstream.h"
namespace tiny{

typedef struct {
    tiny_unsigned_t port;
    tiny_string_t logpath;
    logger::Level loglevel;
    tiny_string_t assets;
    tiny_string_t tpl;
}tiny_conf_t;

tiny_int_t tiny_file_read(const tiny_string_t &, tiny_string_t &);
tiny_array_string_t tiny_file_readlines(const tiny_string_t &);
tiny_conf_t tiny_read_conf(const tiny_string_t &);
std::ostream &operator<<(std::ostream &, tiny_conf_t const &);   

}
