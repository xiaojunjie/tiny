#include "tiny.h"
#include <sstream>
#include <regex>  
#include <iostream>
#include <unistd.h>
#include <time.h>

namespace tiny {

const string Tiny::version = "1.0";

Tiny::Tiny(const string & filename) {
	vector<string> cfgs = Template::ReadFileLines(filename);
	for(auto const& item : cfgs){
		cmatch cm;
		regex e("^(\\S*)\\s*\\=\\s*(\\S*)$");
		regex_match(item.c_str(),cm,e);
		if(cm.size()==3){
			ServerConfig.insert ( pair<string,string>(cm[1],cm[2]) );
		}
	}
    logger::Level loglevel = (logger::Level) atoi(ServerConfig["loglevel"].c_str());
    logger::init(ServerConfig["logpath"], loglevel);
    logger::info << "Tiny Web Server "<< version;
	for(auto const& item: ServerConfig){
		logger::info << "[Config]" << item.first << " = " << item.second;
	}
	Template::Dir = ServerConfig["template"];
    socketQueue = new Sbuf<SocketStream>();
    worker = new ThreadPool<Tiny>(this);
    router = new Route([&](vector<string> argv) {
        Template tpl(Template::TPL404);
        return new HttpResponse(tpl.render(), 404);
    });
	//static file, css, js ...
	route(Route::STATIC_FILES);
}

Tiny::~Tiny() {
    delete router;
    delete worker;
    delete socketQueue;
    logger::info << "Tiny Server Shutdown...";
    logger::destroy();
}

int Tiny::run() {
	int port = atoi(ServerConfig["port"].c_str());
	if(port > 0)
		return run(port);
	else
		return -1;
}

int Tiny::run(int port) {
    return SocketStream::Wait(port, [this](int fd, struct sockaddr_in addr) {
        SocketStream *socket = new SocketStream(fd, addr);
        if(socket == NULL) {
            logger::warm << "SocketStream error";
            return;
        }
        int len = socketQueue->insert(socket);
        if(len >= LISTENQ_G) {
            worker->extend(this);
        } else if(len <= 1) {
            worker->halve<Sbuf<SocketStream>>(socketQueue);
            router->sort();
        }
    });
}

void* Tiny::work(void *args) {
    while (1) {
        SocketStream *socket = socketQueue->remove();
        if(socket == NULL && worker->remove((bool*)args) ){
            return NULL;
        }else{
            logger::debug << "work for fd " << socket->fd;
            HttpRequest *request = new HttpRequest();
            parse(socket, request);
            reply(socket, route(request));
            delete request;
            delete socket;
        }
    }
}

int Tiny::reply(SocketStream *socket, shared_ptr<HttpResponse> response) {
    if(response==NULL){
        return -1;
    }
    logger::debug << "[Tiny::reply] " << *response;
    return reply(socket, response.get());
}

int Tiny::reply(SocketStream *socket, const HttpResponse *response) {
    if(response == NULL || socket == NULL) {
        return -1;
    } else if(HttpProtocol::SendMessage(*socket, *response) < 0) {
        logger::error << "[Tiny] Reply Error " << *response;
    }
    return 1;
}
// return res for req
shared_ptr<HttpResponse> Tiny::route(HttpRequest *request) {
    if(request == NULL) {
        return NULL;
    }

    route_t route_map;
    int status = router->assign(*request,route_map);

    if(status > -1) {
        if(route_map.is_static) {
            vector<string> args = {request->GetUri()};
            return shared_ptr<HttpResponse>( route_map.make_response(args) );
        } else {
            vector<string> args;
            for(const auto &item :route_map.query) {
                string value = request->GetQuery(item);
                args.emplace_back(value);
            }
			args.emplace_back(request->GetBody());
            return shared_ptr<HttpResponse>( route_map.make_response(args) );
        }
    }
    return NULL;
}
// route table
Tiny& Tiny::route(const RouterTable & table){
	for(const auto & item: table){
		route(item.first,item.second);
	}
    return *this;
}
// list of uri
Tiny& Tiny::route(initializer_list<string> uris,  const make_response_function &callback) {
     for(auto const& uri : uris) {
        router->append(uri, callback, false);
    }
    return *this;
}

//css js ...
Tiny& Tiny::route(initializer_list<string> list) {
	string folder = ServerConfig["assets"];
    for(const string & suffix : list) {
        router->append(suffix, [folder, suffix](vector<string> args) {
            if(args.size() >= 1){
                return new HttpResponse(Template::ReadFile(folder + args[0]), 
						HttpMessage::GetType(suffix));
			}else{
                return new HttpResponse();
			}
        }, true);
    }
    return *this;
}

// single uri
Tiny& Tiny::route(string uri, const make_response_function &callback) {
    router->append(uri, callback, false);
    return *this;
}

int Tiny::parse(SocketStream *socket, HttpRequest *request) {
    if(request == NULL || socket == NULL) {
        logger::info << " [parse error] " << *request;
    } else if(HttpProtocol::ParseFirstLine(*socket, *request) < 1) {
        logger::info << " [parse error 0] " << *request;
    } else if(HttpProtocol::ParseHeader(*socket, *request) < 1) {
        logger::info << " [parse error 1] " << *request;
    } else if(HttpProtocol::ParseBody(*socket, *request) < 0) {
        logger::info << " [parse error 2] " << *request;
    }
    logger::debug << "[Tiny::parse] " << *request;
    return 1;	
}

}
