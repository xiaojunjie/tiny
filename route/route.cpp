#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "route.h"
#include "file.h"
using namespace std;
namespace tiny{

	static vector<string> split(const string &s, char delim) {
		vector<string> elems;
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}
	static set<string> keys(const query_t &data){
		set<string> keys;
		for (query_t::const_iterator iter = data.begin();
				iter != data.end();
				++iter)
		{
			keys.insert(iter->first);
		}
		return keys;
	}
	ostream &operator<<(ostream &out, route_t const &route){
		return out << "uri: " << route.uri << ", "
			<< "query: " << route.query.size() << ", "
			<< "static: " << route.is_static << ", "
			<< "count: " << route.count << ".";
	}
	const initializer_list<string> Route::STATIC_FILES = {"css","js","xml","icon","png","jpg","gif","txt"};

	Route::Route(const make_response_function &callback){
		header = new route_t({
				string(),
				vector<string>(),
				callback,
				true,
				NULL,
				0
				});
		tail = header;
		size = 1;
	}
	Route::~Route(){
		route_t *p,*q;
		q = header;
		while (q!=NULL) {
			p = q->next;
			delete q;
			q = p;
		}
	}
	int Route::append(const string &uri, const make_response_function &callback, bool is_static){
		vector<string> query;
		string path;
		const char* pos = strrchr(uri.c_str(),'?');
		if(pos!=NULL){
			string str(pos+1);
			query = Route::ParseQuery(str);
			path = uri.substr(0, uri.length()-str.length()-1 );
		}else{
			path = uri;
		}
		if( !is_static && path.back() != '/' )
			path += '/';
		tail->next = new route_t({
				path,
				query,
				callback,
				is_static,
				NULL,
				0
				});
		logger::debug << "[New Route] " << *tail->next << logger::endl;
		tail = tail->next;
		return ++size;
	}

	int Route::sort(route_t * item){
		// 访问次数大于item的点
		route_t *p = header;
		while (p->next->count > item->count) {
			p = p->next;
		}
		// item的前点
		route_t *q = p;
		while (q->next!=item) {
			q = q->next;
		}
		if(p!=q){
			q->next = item->next;
			item->next = p->next;
			p->next = item;
		}
		while (tail->next != NULL) {
			tail = tail->next;
		}
		return 1;
	}
	const route_t * Route::assign(const HttpRequest &request){
		route_t *p = header->next;
		while (p!=NULL) {
			if(Match(p,request)){
				p->count++;
				sort(p);
				return p;
			}
			p = p->next;
		}
		if(MatchType(header,request))
			return header;
		return NULL;
	}
	vector<string> Route::ParseQuery(const string &uri){
		vector<string> query;
		for(const auto &item: split(uri,'&')){
			if(item.back()=='='){
				string key = item.substr(0,item.size()-1);
				query.push_back(key);
			}
		}
		return query;
	}
	string Route::ParseSuffix(const HttpRequest &request){
		string uri = request.GetUri();
		const char* pos = strrchr(uri.c_str(),'.');
		if(pos!=NULL){
			return string(pos+1);
		}else{
			return string();
		}
	}

	set<string> Route::ParseType(const HttpRequest &request){
		string types = request.GetHeader(HttpMessage::HEADER_ACCEPT);
		if (types.find(HttpMessage::TYPE_ALL) != string::npos)
			return set<string>({HttpMessage::TYPE_ALL});
		else{
			set<string> result;
			for( const auto &item: split(types,';') ){
				vector<string> type = split(item,',');
				result.insert(type.begin(),type.end());
			}
			return result;
		}

		// string uri = request.GetUri();
		// if(uri.back()=='/'){
		//     return types.find("html")->second;
		// }else if(!uri.empty()){
		//     const char* pos = strrchr(uri.c_str(),'.');
		//     if(pos!=NULL){
		//         string str(pos+1);
		//         auto p = types.find( str );
		//         if(p!=types.end())
		//             return p->second;
		//     }
		// }
		// return HttpMessage::TYPE_TEXT_PLAIN;
	}
	bool Route::MatchUri(const route_t * route, const string &request){
		//std::cout << 1 << std::endl;
		return route->is_static || // skip if static
			route->uri == request ||
			(request.back() != '/' && // request end without /
			request.size()+1 == route->uri.size() &&
			request+"/" == route->uri);

	}
	bool Route::MatchQuery(const vector<string> &a, const set<string> &b){
		//std::cout << 2 << std::endl;
		return a.empty() || set<string>(a.begin(),a.end())==b;
	}
	bool Route::MatchType(const route_t * route, const HttpRequest &request){
		//std::cout << 3 << std::endl;
		if(route->is_static){
			return route->uri == ParseSuffix(request);
		}else{
			return true;
			// set<string> types_of_request = ParseType(request);
			// string type_of_response = response->GetHeader(HttpMessage::HEADER_CONTENT_TYPE);
			// return types_of_request.find(HttpMessage::TYPE_ALL) != types_of_request.end()
			//     || types_of_request.find(type_of_response) != types_of_request.end();
		}
	}
	bool Route::Match(const route_t * item, const HttpRequest &request){
		return MatchUri(item, request.GetUri())
			&& MatchQuery( item->query, keys( request.GetQuery() ) )
			&& MatchType(item, request);
	}

}
