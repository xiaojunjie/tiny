#pragma once

#include "tiny_base.h"
#include "tiny_handler.h"
namespace tiny{

struct tiny_router_conf_t{
    // flag
    // 0 find 
    // 1 any
    // 2 equal
    tiny_string_t uri;
    tiny_http_handler_pt handler;
    tiny_flag_t flag;
    tiny_unsigned_t count;
    tiny_router_conf_t(tiny_http_handler_pt handler):
        tiny_router_conf_t("",handler,1)
    {}
    tiny_router_conf_t(tiny_string_t uri, tiny_http_handler_pt handler):
        tiny_router_conf_t(uri,handler,0)
    {}
    tiny_router_conf_t(tiny_string_t uri, tiny_http_handler_pt handler, tiny_flag_t tag):
        uri(std::move(uri)),
        handler(handler),
        flag(tag),
        count(0)
    {}
    tiny_bool_t match(const tiny_http_request_t & request){
        tiny_bool_t result;
        switch(flag){
            case 0:
                result = request.uri.find(uri)==0;
                count += result;
                break;
            case 1:
                result = TINY_TRUE;
                break;
            case 2:
                result = uri.compare(request.uri)==0;
                count += result;
                break;
            default:
                result = TINY_FALSE;
        }
        return result;
    }
};
typedef std::vector< tiny_router_conf_t > tiny_router_t;
tiny_int_t tiny_router_append(tiny_router_t &, tiny_string_t, tiny_http_handler_pt);
std::ostream &operator<<(std::ostream &, const tiny_router_conf_t&);

}
