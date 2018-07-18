#pragma once

#include <sstream>
#include <string>
#include <map>
using namespace std;
namespace tiny{

typedef map<string,string> http_header_t;
typedef map<string,string> query_t;
enum Status{Ready,FirstLine,Header,Body,Over,Error};

class HttpMessage{
public:
    static const string HEADER_CONTENT_LENGTH;
    static const string HEADER_TRANSFER_ENCODING;
    static const string HEADER_CONTENT_TYPE;
    static const string HEADER_SERVER;
    static const string HEADER_ACCEPT;
    static const string HEADER_CONNECTION;
    static const string HttpVersion;
    static const string MessageOK;
    static const string TYPE_ALL;
    static const string TYPE_TEXT_HTML;
    static const string TYPE_TEXT_PLAIN;
    static const string TYPE_TEXT_CSS;
    static const string TYPE_IMAGE_GIF;
    static const string TYPE_IMAGE_PNG;
    static const string TYPE_IMAGE_ICO;
    static const string TYPE_IMAGE_JEPG;
    static const string TYPE_APPLICATION_JS;
    static const string TYPE_APPLICATION_JSON;
    static const string TYPE_APPLICATION_XML;
    int SetHeader(string, string);
    int SetHeader(string, int);
    string GetHeader(string) const;
    virtual string GetFirstLine() const = 0;
	//virtual ~HttpMessage() = 0;
    friend ostream &operator<<(ostream &, HttpMessage const &);
    static string GetType(string);
protected:
    http_header_t header;
    string body;
    string version;
};

class HttpRequest:public HttpMessage{
public:
	HttpRequest();
	~HttpRequest();
    int SetVersion(string);
    int SetUri(string);
    int SetMethod(string);
    int SetIp(string);
    int SetQuery(string,string);
    int SetBody(const char*);
    string GetUri() const;
    string GetMethod() const;
    string GetBody() const;
    string GetFirstLine() const override;
    const query_t GetQuery() const;
    string GetQuery(string) const;
    Status status;
private:
    string ip;
    string method;
    string uri;
    string version;
    query_t query;
};

class HttpResponse:public HttpMessage{
public:
	HttpResponse() = default;
    HttpResponse(const HttpResponse &);
    HttpResponse(string);
    HttpResponse(string,string);
    HttpResponse(string,int);
    HttpResponse(string,string,int);
    string GetBody() const{
        return body;
    };
    string GetFirstLine() const override;
private:
    string message;
    int stat;
};

ostream &operator<<(ostream &, HttpMessage const &);

}
