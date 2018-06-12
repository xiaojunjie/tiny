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
	const initializer_list<string> Route::STATIC_FILES = {"html","css","js","xml","ico","png","jpg","gif","txt"};

	Route::Route(const make_response_function &callback){
        default_route.make_response = callback;
        default_route.is_static = true;
        default_route.method = "GET";
	}
	Route::~Route(){
        for(auto& p: route_list){
            delete p;
        }
	}
	int Route::append(const string& uri,
                      const make_response_function& callback,
                      bool is_static, 
                      const string& method){
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
        route_list.emplace_back(new route_t(
		    path,
			query,
			callback,
			is_static,
            method
        ));
		logger::debug << "[New Route] " << *route_list.back();
		return route_list.size();
	}

	int Route::sort(){
        typedef pair<unsigned,unsigned> kv_t;
        unsigned n = route_list.size();
        vector<kv_t> kvs;
        mutex_sort.lock();
        for(unsigned i=0; i<n; i++){
           kvs.emplace_back(i,route_list[i]->count);
        }
        mutex_sort.unlock();
        // insert sort
        for (int j = 1; j < n; j++){
            kv_t obj = kvs[j]; 
            //if(obj.second==0)
            //    continue;
            int i = j - 1;  
            while (i >= 0 && obj.second > kvs[i].second){
                kvs[i + 1] = kvs[i];
                i--;                 
            }
            kvs[i + 1] = obj;
        }
        vector<route_t*> backup(n);
        for(int i=0; i<n; i++){
            unsigned index = kvs[i].first;
            backup[i] = route_list[index];
        }
        mutex_sort.lock();
        route_list = backup;
        mutex_sort.unlock();
        logger::debug << "[Route] list";
        for(const auto& p: backup){
            logger::debug << *p;
        }
		return 1;
	}
    mutex Route::mutex_sort;
	int Route::assign(const HttpRequest& request, route_t& result){
        static int count = 0;
        static mutex mutex_count;
        static vector<mutex> mutex_list(route_list.size());
        mutex_count.lock();
        if(count==0)
             mutex_sort.lock(); //avoid sort
        count++;
        mutex_count.unlock();
        
        int status = -1;
        int size = route_list.size();
        for(int i=0; i<size; i++){
            if(Match(*route_list[i],request)){
                mutex_list[i].lock();
                route_list[i]->count++;
                mutex_list[i].unlock();
                result = *route_list[i];
                status = i;
            }
        }
        if(status<0 && MatchType(default_route,request)){
            result = default_route;
            status = route_list.size();
        }

        mutex_count.lock();
        count--;
        if(count==0)
             mutex_sort.unlock(); // arrow sort
        mutex_count.unlock();
        return status;
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
	bool Route::MatchUri(const route_t& route, const string &request){
		//std::cout << 1 << std::endl;
		return route.is_static || // skip if static
			route.uri == request ||
			(request.back() != '/' && // request end without /
			request.size()+1 == route.uri.size() &&
			request+"/" == route.uri);

	}
	bool Route::MatchQuery(const vector<string> &a, const set<string> &b){
		//==;
		//return a.empty() || set<string>(a.begin(),a.end())==b;
		// >=
		return a.empty() || includes(a.begin(),a.end(),b.end(),b.end());
	}
	bool Route::MatchType(const route_t& route, const HttpRequest &request){
		if(route.is_static){
			return route.uri == ParseSuffix(request);
		}else{
			return route.method == request.GetMethod();
			// set<string> types_of_request = ParseType(request);
			// string type_of_response = response->GetHeader(HttpMessage::HEADER_CONTENT_TYPE);
			// return types_of_request.find(HttpMessage::TYPE_ALL) != types_of_request.end()
			//     || types_of_request.find(type_of_response) != types_of_request.end();
		}
	}
	bool Route::Match(const route_t& item, const HttpRequest &request){
		return MatchUri(item, request.GetUri())
			&& MatchQuery( item.query, keys( request.GetQuery() ) )
			&& MatchType(item, request);
	}

}
