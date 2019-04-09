#include "tiny_file_io.h"
namespace tiny{

tiny_int_t tiny_file_read(const tiny_string_t &filename, tiny_string_t &userbuf){
    std::ifstream file("./"+filename, std::ifstream::binary | std::ifstream::in);
    if(!file)
        return TINY_ERROR;
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    userbuf = buffer.str();
    return TINY_SUCCESS;
}

tiny_array_string_t tiny_file_readlines(const tiny_string_t & filename){
	tiny_array_string_t elems;
	tiny_string_t content;
    int n = tiny_file_read(filename, content);
    std::stringstream ss(content);
	tiny_string_t item;
	while (getline(ss, item)) {
		elems.push_back(item);
	}
	return elems;
}

}
