#include "http_protocol.h"
#include <algorithm>  
#include <iostream>
namespace tiny{
    static constexpr auto HEADER_CONTENT_LENGTH = "content-length";
    static constexpr auto HEADER_TRANSFER_ENCODING = "transfer-encoding";
    static constexpr auto HEADER_CONTENT_TYPE = "content-type";
    static constexpr auto HEADER_SERVER = "server";
    static constexpr auto HEADER_ACCEPT = "accept";
    static constexpr auto HEADER_COOKIE = "cookie";
    static constexpr auto HEADER_REFERER = "referer";
    static constexpr auto HEADER_USER_AGENT = "user-agent";
    static constexpr auto HEADER_HOST = "host";
    static constexpr auto HEADER_CACHE_CONTROL = "cache_control";
    static constexpr auto HEADER_PRAGMA = "pragma";
    static constexpr auto HEADER_LOCATION = "location";
    static constexpr auto HEADER_CONNECTION = "connection";
    static constexpr auto MessageOK = "Ok";
    static constexpr auto MessageNOTFOUND = "Not Found";
    static constexpr auto MessageREDIRECT = "Temporary Redirect";
    static constexpr auto HttpVersion = "HTTP/1.1";
    static constexpr auto TYPE_ALL = "*/*";
    static constexpr auto TYPE_TEXT_HTML = "text/html";
    static constexpr auto TYPE_TEXT_CSS = "text/css";
    static constexpr auto TYPE_IMAGE_ICO = "image/x-icon";
    static constexpr auto TYPE_TEXT_PLAIN = "text/plain";
    static constexpr auto TYPE_IMAGE_GIF = "image/gif";
    static constexpr auto TYPE_IMAGE_PNG = "image/png";
    static constexpr auto TYPE_IMAGE_JEPG = "image/jepg";
    static constexpr auto TYPE_APPLICATION_JS = "application/x-javascript";
    static constexpr auto TYPE_APPLICATION_JSON = "application/json";
    static constexpr auto TYPE_APPLICATION_XML = "application/xml";
    static constexpr auto CONNEXTION_KEEP_ALIVE = "keep-alive";
    static constexpr auto CONNEXTION_CLOSE = "close";
    tiny_dict_t tiny_http_file_types = {
        {"html",TYPE_TEXT_HTML},
        {"json",TYPE_APPLICATION_JSON},
        {"gif",TYPE_IMAGE_GIF},
        {"png",TYPE_IMAGE_PNG},
        {"jpg",TYPE_IMAGE_JEPG},
        {"ico",TYPE_IMAGE_ICO},
        {"css",TYPE_TEXT_CSS},
        {"js",TYPE_APPLICATION_JS},
        {"xml",TYPE_APPLICATION_XML}
    };
    std::ostream &operator<<(std::ostream &out, tiny_http_request_t const &request){
        std::ostringstream buf;
        buf << request.method << " "
             << request.uri << " "
             << request.protocol << "\r\n";
        buf << request.header;
        buf << "\r\n";
        buf << request.body;
        return out << buf.str();
    }
    std::ostream &operator<<(std::ostream &out, http_request_header_t const &header){
        std::ostringstream buf;
        buf << HEADER_CONTENT_LENGTH  << ": " << header.content_length << "\r\n";
        buf << HEADER_TRANSFER_ENCODING << ": " << header.transfer_encoding << "\r\n";
        buf << HEADER_ACCEPT << ": " << header.accept << "\r\n";
        buf << HEADER_CONNECTION << ": " << header.connection << "\r\n";
        buf << HEADER_COOKIE  << ": " << header.cookie << "\r\n";
        buf << HEADER_REFERER << ": " << header.referer << "\r\n";
        buf << HEADER_USER_AGENT << ": " << header.user_agent << "\r\n";
        buf << HEADER_HOST << ": " << header.host << "\r\n";
        buf << HEADER_CACHE_CONTROL << ": " << header.cache_control << "\r\n";
        buf << HEADER_PRAGMA << ": " << header.pragma << "\r\n";
        return out << buf.str();
    }
    tiny_http_response_t &operator<<(tiny_http_response_t & response, const tiny_string_t & str){
        response.body += str;
        return response;
    }
    std::ostream &operator<<(std::ostream &out, tiny_http_response_t const &response){
        std::ostringstream buf;
        buf << response.protocol << " "
             << response.status << " "
             << response.message << "\r\n";
        buf << response.header;
        buf << "\r\n";
        buf << response.body;
        return out << buf.str();
    }
    std::ostream &operator<<(std::ostream &out, http_response_header_t const &header){
        std::ostringstream buf;
        buf << HEADER_CONTENT_LENGTH << ": " << header.content_length << "\r\n";
        buf << HEADER_CONTENT_TYPE << ": " << header.content_type << "\r\n";
        buf << HEADER_SERVER << ": " << header.server << "\r\n";
        buf << HEADER_CONNECTION << ": " << header.connection << "\r\n";
        buf << HEADER_COOKIE << ": " << header.cookie << "\r\n";
        buf << HEADER_LOCATION << ": " << header.location << "\r\n";
        buf << HEADER_CACHE_CONTROL << ": " << header.cache_control << "\r\n";
        return out << buf.str();
    }
    static tiny_array_string_t split(const tiny_string_t &s, char delim) {
        tiny_array_string_t elems;
        std::stringstream ss(s);
        tiny_string_t item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        if(s.back() == delim)
            elems.push_back("");
        return elems;
    }
    tiny_int_t http_parse(tiny_http_request_t &request, const tiny_string_t &buf){
        if(
                http_parse_request_line(buf, request)==TINY_ERROR || 
                http_parse_header(buf, request)==TINY_ERROR || 
                http_parse_body(buf, request)==TINY_ERROR
          )
            return TINY_ERROR;
        else
            return TINY_SUCCESS;
    }
    tiny_int_t http_parse_request_line(const tiny_string_t &buf, tiny_http_request_t &request){
        int length = buf.find("\r\n");
        if(length<0)
            return TINY_ERROR;
        tiny_array_string_t header = split(buf.substr(0,length), ' ');
        if(header.size()==3){
            request.method = header[0];
            request.uri = header[1];
            request.protocol = header[2];
        }else{
            return TINY_ERROR;
        }
        return TINY_SUCCESS;
    }
    tiny_int_t http_parse_header(const tiny_string_t &buf, tiny_http_request_t &request){
        tiny_int_t length = buf.length();
        if(length==0)
            return TINY_ERROR;
        tiny_dict_t header;
        for(int i=buf.find("\r\n")+2, j=0; i<length; i=j+2){
            j = buf.find("\r\n", i);
            if(j<=i)
                break;
            tiny_string_t item = buf.substr(i,j-i);
            int p = item.find(": ");
            if(p<0)
                continue;
            tiny_string_t key = item.substr(0,p);
            tiny_string_t val = item.substr(p+2);
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            header.insert(tiny_kv_t(key,val));
        }

        auto kv = header.find(HEADER_CONTENT_LENGTH);
        if(kv!=header.end())
            request.header.content_length = kv->second;

        kv = header.find(HEADER_TRANSFER_ENCODING);
        if(kv!=header.end())
            request.header.transfer_encoding = kv->second;

        kv = header.find(HEADER_ACCEPT);
        if(kv!=header.end())
            request.header.accept = kv->second;

        kv = header.find(HEADER_CONNECTION);
        if(kv!=header.end())
            request.header.connection = kv->second;

        kv = header.find(HEADER_COOKIE);
        if(kv!=header.end())
            request.header.cookie = kv->second;

        kv = header.find(HEADER_REFERER);
        if(kv!=header.end())
            request.header.referer = kv->second;
        
        kv = header.find(HEADER_USER_AGENT);
        if(kv!=header.end())
            request.header.user_agent = kv->second;

        kv = header.find(HEADER_HOST);
        if(kv!=header.end())
            request.header.host = kv->second;

        kv = header.find(HEADER_CACHE_CONTROL);
        if(kv!=header.end())
            request.header.cache_control = kv->second;

        kv = header.find(HEADER_PRAGMA);
        if(kv!=header.end())
            request.header.pragma = kv->second;
        if(header.size()>0)
            return TINY_SUCCESS;
        else
            return TINY_ERROR;
    }

    tiny_int_t http_parse_body(const tiny_string_t &buf, tiny_http_request_t &request){
        if(request.header.content_length.empty())
            return TINY_OVER;
        unsigned int length = std::stoi(request.header.content_length);
        if(request.body.empty()){
            int i = buf.find("\r\n\r\n");
            if(i>=0)
                request.body = buf.substr(i+4);
            else
                request.body = buf;
        }else{
            request.body += buf;
        }
        if(length > request.body.length())
            return TINY_ERROR;
        else
            return TINY_SUCCESS;
    }
    tiny_bool_t http_connnecton_status(const tiny_http_response_t & response){
        return response.header.connection == CONNEXTION_KEEP_ALIVE;
    }
    tiny_bool_t http_request_status(const tiny_http_request_t & request){
        if(request.method.empty())
            return false;
        if(request.header.content_length.empty())
            return true;
        return request.body.length() == (unsigned int)stoi(request.header.content_length);
    }
    tiny_int_t http_connection_handler(const tiny_http_request_t &request, tiny_http_response_t &response){
        response.header.connection = request.header.connection;
        //response.header.connection = CONNEXTION_CLOSE;
        return TINY_SUCCESS;
    }
    tiny_int_t http_version_handler(tiny_http_response_t &response){
        response.protocol = HttpVersion;
        return TINY_SUCCESS;
    }
    tiny_int_t http_cookie_handler(const tiny_http_request_t &request, tiny_http_response_t &response){
        response.header.cookie = request.header.cookie;
        return TINY_SUCCESS;
    }
    tiny_int_t http_status_handler(tiny_http_response_t &response, tiny_int_t status){
        response.status = status;
        switch(status){
            case TINY_HTTP_OK:
                response.message = MessageOK;
                break;
            case TINY_HTTP_NOTFOUND:
                response.message = MessageNOTFOUND;
                break;
            default:
                return TINY_ERROR;
        }
        return TINY_SUCCESS;
    }
}
