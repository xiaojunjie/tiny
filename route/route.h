#pragma once

#include <initializer_list>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <mutex>
#include "http.h"
using namespace std;
namespace tiny{

typedef HttpResponse*(*tiny_http_handler_pt)(const HttpRequest*);
typedef function<HttpResponse*(vector<string>)> make_response_function;

struct route_t{
    string uri;
    vector<string> query;
    make_response_function make_response;
    bool is_static;
    string method;
    unsigned count;
    route_t():count(0){}
    route_t(string uri, 
            vector<string> query, 
            make_response_function make_response,
            bool is_static,
            string method): 
            uri(uri), 
            query(query),
            make_response(make_response),
            is_static(is_static),
            method(method),
            count(0){}
};
ostream &operator<<(ostream &, route_t const &);

class Route{
public:
    Route(const make_response_function &);
    ~Route();
    vector<route_t*> route_list;
    static mutex mutex_sort;
    int append(const std::string &, tiny_http_handler_pt);
    int append(const std::string &,
               const make_response_function &, 
               bool is_static = false, 
               const std::string& method = "GET");
    int assign(const HttpRequest &, route_t &);
    int sort();
    HttpResponse* Get404Response();
    static set<string> ParseType(const HttpRequest &); // for request
    static string ParseSuffix(const HttpRequest &); // for request
    static vector<string> ParseQuery(const string &); // for self
    static const initializer_list<string> STATIC_FILES;
private:
    route_t default_route;
    static bool Match(const route_t&, const HttpRequest &);
    static bool MatchUri(const route_t&, const string&);
    static bool MatchQuery(const vector<string> &, const set<string> &);
    static bool MatchType(const route_t& route, const HttpRequest &);
};

}
