#include "http_message.h"
#include <algorithm>
#include <iostream>
namespace tiny{

const string HttpMessage::HEADER_CONTENT_LENGTH = "content-length";
const string HttpMessage::HEADER_TRANSFER_ENCODING = "transfer-encoding";
const string HttpMessage::HEADER_CONTENT_TYPE = "content-type";
const string HttpMessage::HEADER_SERVER = "server";
const string HttpMessage::HEADER_ACCEPT = "accept";
const string HttpMessage::MessageOK = "Ok";
const string HttpMessage::HttpVersion = "HTTP/1.0";
const string HttpMessage::TYPE_ALL = "*/*";
const string HttpMessage::TYPE_TEXT_HTML = "text/html";
const string HttpMessage::TYPE_TEXT_CSS = "text/css";
const string HttpMessage::TYPE_IMAGE_ICO = "image/x-icon";
const string HttpMessage::TYPE_TEXT_PLAIN = "text/plain";
const string HttpMessage::TYPE_IMAGE_GIF = "image/gif";
const string HttpMessage::TYPE_IMAGE_PNG = "image/png";
const string HttpMessage::TYPE_IMAGE_JEPG = "image/jepg";
const string HttpMessage::TYPE_APPLICATION_JS = "application/x-javascript";
const string HttpMessage::TYPE_APPLICATION_JSON = "application/json";
const string HttpMessage::TYPE_APPLICATION_XML = "application/xml";
std::ostream &operator<<(std::ostream &out, HttpMessage const &message) {
    ostringstream buf;
    buf << message.GetFirstLine() << "\r\n";
    for(const auto &item: message.header){
        buf << item.first << ": " << item.second << "\r\n";
    }
    buf << "\r\n";
    buf << message.body;
    return out << buf.str();
}
std::ostream &operator<<(std::ostream &out, query_t const &query) {
    ostringstream buf;
	if(query.size()>0){
		buf << "?";
   		for(const auto &item: query){
        	buf << item.first << "=" << item.second << "&";
    	}
	}
    return out << buf.str();
}

string HttpMessage::GetType(string suffix){
    const static map<string,string> types = {
        {"html",TYPE_TEXT_HTML},
        {"gif",TYPE_IMAGE_GIF},
        {"png",TYPE_IMAGE_PNG},
        {"jpg",TYPE_IMAGE_JEPG},
        {"ico",TYPE_IMAGE_ICO},
        {"css",TYPE_TEXT_CSS},
        {"js",TYPE_APPLICATION_JS},
        {"xml",TYPE_APPLICATION_XML}
    };
    auto p = types.find( suffix );
    if( p!=types.end() )
        return p->second;
    else
        return TYPE_TEXT_PLAIN;
}

int HttpMessage::SetHeader(string key, string value){
    header[key] = value;
    return 1;
}
int HttpMessage::SetHeader(string key, int value){
    SetHeader(key,to_string(value));
    return 1;
}
string HttpMessage::GetHeader(string key) const{
    auto p = header.find(key);
    if(p!=header.end())
        return p->second;
    else
        return "";
}
string HttpRequest::GetFirstLine() const{
    ostringstream line;
    line << method << " "
         << uri  << query << " "
         << version;
    return line.str();
}
int HttpRequest::SetVersion(string str){
    version = str;
    return version.length();
}
int HttpRequest::SetUri(string str){
   	uri = str;
    return uri.length();
}
int HttpRequest::SetMethod(string str){
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    method = str;
    return method.length();
}
int HttpRequest::SetQuery(string k, string v){
    query[k] = v;
    return query.size();
}
int HttpRequest::SetIp(string str){
    ip = str;
    return ip.length();
}
int HttpRequest::SetBody(const char* str){
    body = string(str);
    return body.length();
}
const query_t HttpRequest::GetQuery() const{
    return query;
}
string HttpRequest::GetQuery(string key) const{
    auto p = query.find(key);
    if(p!=query.end())
        return p->second;
    else
        return string();
}
string HttpRequest::GetMethod() const{
    return method;
}
string HttpRequest::GetBody() const{
    return body;
}
string HttpRequest::GetUri() const{
    return uri;
}
HttpResponse::HttpResponse(string str):
    HttpResponse(str,HttpMessage::TYPE_TEXT_HTML,200){}
HttpResponse::HttpResponse(string str,string type):
    HttpResponse(str,type,200){}
HttpResponse::HttpResponse(string str,int code):
    HttpResponse(str,HttpMessage::TYPE_TEXT_HTML,code){}
HttpResponse::HttpResponse(string str, string type, int code){
    body = str;
    stat = code;
    version = HttpMessage::HttpVersion;
    message = HttpMessage::MessageOK;
    SetHeader(HttpMessage::HEADER_SERVER, "Tiny Web Server");
    SetHeader(HttpMessage::HEADER_CONTENT_LENGTH, body.length());
    SetHeader(HttpMessage::HEADER_CONTENT_TYPE, type);
}
string HttpResponse::GetFirstLine() const{
    ostringstream line;
    line << version << " "
         << stat << " "
         << message;
    return line.str();
}
}
