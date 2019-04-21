#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#define TINY_SUCCESS 1
#define TINY_OVER 0
#define TINY_ERROR -1
#define TINY_TRUE true
#define TINY_FALSE false
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
typedef uint8_t tiny_flag_t;
typedef int tiny_int_t;
typedef unsigned int tiny_unsigned_t;
typedef bool tiny_bool_t;
typedef char tiny_char_t;
typedef std::string tiny_string_t;
typedef std::vector<tiny_string_t> tiny_array_string_t;
typedef std::unordered_set<tiny_string_t> tiny_set_string_t;
typedef std::pair<tiny_string_t,tiny_string_t> tiny_kv_t;
typedef std::unordered_map<tiny_string_t,tiny_string_t> tiny_dict_t;
