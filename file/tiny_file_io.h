#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include "tiny_base.h"
namespace tiny{
tiny_int_t tiny_file_read(const tiny_string_t &, tiny_string_t &);
tiny_array_string_t tiny_file_readlines(const tiny_string_t &);

}
