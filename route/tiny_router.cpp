#include "tiny_router.h"
#include "file/file_logger.h"
#include <algorithm>
#include <string.h>
using namespace std;
namespace tiny {

std::ostream &operator<<(std::ostream &out, const tiny_router_conf_t &item) {
    return out << "uri: " << item.uri << ", "
               << "flag: " << item.flag << ", "
               << "count: " << item.count << ".";
}

tiny_int_t tiny_router_append(tiny_router_t &table, tiny_string_t uri,
                              tiny_http_handler_pt handler) {
    table.emplace_back(uri, handler);
    logger::debug << "[New Route] " << table.back();
    return table.size();
}

}
