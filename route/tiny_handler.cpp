#include "tiny_handler.h"
#include "tiny_file.h"
namespace tiny {
extern tiny_dict_t tiny_http_file_types;
static tiny_string_t parse_suffix(tiny_string_t uri) {
    tiny_int_t pos = uri.find_last_of('.');
    if (pos<0)
        return "";
    else
        return uri.substr(pos+1);
}

static tiny_string_t get_type(tiny_string_t filename) {
    tiny_string_t suffix = parse_suffix(filename);
    auto p = tiny_http_file_types.find(suffix);
    if (p != tiny_http_file_types.end())
        return p->second;
    else
        return "";
}

tiny_int_t tiny_http_init_handler(const tiny_http_request_t &request,
        tiny_http_response_t &response) {
    response.header.content_length = 0;
    response.status = TINY_HTTP_NOTFOUND;
    response.header.server = "Tiny";
    return TINY_SUCCESS;
}
tiny_int_t tiny_http_base_handler(const tiny_http_request_t &request,
                                   tiny_http_response_t &response) {
    tiny_int_t result = TINY_SUCCESS;
    if(response.protocol.empty()){
        result &= http_version_handler(response);
    }
    if(response.status==0){
        result &= http_status_handler(response, 404);
    }
    if(response.header.cookie.empty()){
        result &= http_cookie_handler(request, response);
    }
    if(response.header.connection.empty()){
        result &= http_connettion_handler(request, response);
    }
    response.header.content_length = response.body.length();
    return result;
}

static tiny_int_t tiny_http_machine_over(const tiny_http_event_t & event, 
        tiny_http_task_t & task){
    task.status = http_status_type::over;
    return TINY_SUCCESS;
}

static tiny_int_t tiny_http_machine_header_body(const tiny_http_event_t & event, 
                                            tiny_http_task_t & task){
    if(event.event==http_event_type::close || task.status!=http_status_type::header)
        return tiny_http_machine_over(event, task);
    if(task.request->header.content_length.empty()){
        task.status = http_status_type::body;
        return TINY_SUCCESS;
    }
    const tiny_string_t *p = (tiny_string_t*)event.data;
    unsigned int length = stoi(task.request->header.content_length);
    if(http_parse_body(*p, *task.request)){
        if( length == task.request->body.length() )
            task.status = http_status_type::body;
        return TINY_SUCCESS;
    }else{
        return tiny_http_machine_over(event, task);
    }
}

static tiny_int_t tiny_http_machine_init_header(const tiny_http_event_t & event, 
                                               tiny_http_task_t & task){
    if(event.event==http_event_type::close || task.status!=http_status_type::init)
        return tiny_http_machine_over(event, task);
    const tiny_string_t *p = (tiny_string_t*)event.data;
    int i = p->find("\r\n");
    int j = p->find("\r\n\r\n");
    if (i < 0 || j<0)
        return tiny_http_machine_over(event, task);
    if( http_parse_request_line(p->substr(0, i), *task.request) &&
        http_parse_header(p->substr(i + 2, j - i), *task.request) ){
        task.status = http_status_type::header;
        return tiny_http_machine_header_body(event, task);
    }else{
        return tiny_http_machine_over(event, task);
    }
}

tiny_int_t tiny_http_parse_handler(const tiny_http_event_t & event,
                                   tiny_http_task_t & task) {
    typedef tiny_int_t(*machine)(const tiny_http_event_t &, tiny_http_task_t &);  
    static std::map<http_status_type, machine> machineset = {
        {http_status_type::init,   tiny_http_machine_init_header},
        {http_status_type::header, tiny_http_machine_header_body},
        {http_status_type::body,   tiny_http_machine_over},
        {http_status_type::over,   tiny_http_machine_over}
    };
    auto it = machineset.find(task.status);
    if(it==machineset.end())
        return TINY_ERROR;
    else
        return it->second(event,task);
}

tiny_int_t tiny_http_file_handler(const tiny_string_t& filename, tiny_http_response_t &response) {
    tiny_string_t buf;
    tiny_string_t filetype = get_type(filename);
    tiny_int_t s = tiny_file_read(filename, buf);
    if (s == TINY_SUCCESS) {
        response.status = TINY_HTTP_OK;
        response.header.content_type = filetype;
        response.body = buf;
    } else {
        response.status = TINY_HTTP_NOTFOUND;
    }
    return s;
}

tiny_int_t tiny_http_static_handler(const tiny_http_request_t &request,
                                    tiny_http_response_t &response) {
    tiny_string_t filename = request.uri;
    return tiny_http_file_handler(filename, response);
}

tiny_int_t tiny_http_json_handler(const tiny_string_t & json,
                                    tiny_http_response_t &response) {
        response.status = TINY_HTTP_OK;
        response.header.content_type = get_type("*.json");
        response.body = json;
        return TINY_SUCCESS;
}


tiny_int_t tiny_http_index_handler(const tiny_http_request_t &request,
                                    tiny_http_response_t &response) {
    tiny_string_t filename = "/template/index.tpl";;
    return tiny_http_file_handler(filename, response);
}

tiny_int_t tiny_http_notfound_handler(const tiny_http_request_t &request,
                                    tiny_http_response_t &response) {
    if(response.status != TINY_HTTP_NOTFOUND)
        return TINY_SUCCESS;
    tiny_string_t filename = "/template/404.tpl";
    tiny_int_t s = tiny_http_file_handler(filename, response);
    response.status = TINY_HTTP_NOTFOUND;
    return s;
}

}
