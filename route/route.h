#pragma once

#include <initializer_list>
#include <iostream>
#include <map>
#include <set>
#include "http.h"
using namespace std;
namespace tiny{

typedef function<HttpResponse*(vector<string>)> make_response_function;

struct route_t{
    string uri;
    vector<string> query;
    make_response_function make_response;
    bool is_static;
    route_t *next;
    unsigned count;
};
ostream &operator<<(ostream &, route_t const &);

class Route{
public:
    Route(const make_response_function &);
    ~Route();
    int append(const std::string &,const make_response_function &, bool);
    const route_t * assign(const HttpRequest &);
    static set<string> ParseType(const HttpRequest &); // for request
    static string ParseSuffix(const HttpRequest &); // for request
    static vector<string> ParseQuery(const string &); // for self
    static const initializer_list<string> STATIC_FILES;
private:
    route_t *header;
    route_t *tail;
    int sort(route_t*);
    unsigned size;
    static bool Match(const route_t*, const HttpRequest &);
    static bool MatchUri(const route_t *, const string&);
    static bool MatchQuery(const vector<string> &, const set<string> &);
    static bool MatchType(const route_t * route, const HttpRequest &);
};

}
