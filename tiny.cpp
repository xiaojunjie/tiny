#include "tiny.h"
#include <sstream>
#include <regex>  
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <sys/eventfd.h>

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
    socketQueue = new Sbuf<void>();
    worker = new ThreadPool<Tiny>(this);
    router = new Route([&](vector<string> argv) {
        Template tpl(Template::TPL404);
        HttpResponse *response = new HttpResponse(tpl.render(), 404);
        //response->SetHeader(HttpMessage::HEADER_CONNECTION,"close");
        return response;
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
    return SocketStream::Wait(port, [this]() {
        //SocketStream *socket = new SocketStream(fd,addr);
        //if(socket == NULL) {
        //    logger::warm << "SocketStream error";
        //    return;
        //}
        int len = socketQueue->insert(NULL);
        std::cout << "len: " << len << std::endl;
        //for(const auto& nfd: noticefds){
        //    //std::cout << "noticefds: " << nfd << std::endl;
        //    static uint64_t temp = 1; // temp>0
        //    if( write(nfd,&temp,sizeof(temp))<0 ){
        //        logger::warm << "[Tiny] notice error";
        //    }
        //}
        //if(len >= LISTENQ_G) {
        //    //worker->extend(this);
        //} else if(len <= 1) {
        //    //worker->halve<Sbuf<SocketStream>>(socketQueue);
        //    //router->sort();
        //}
    });
}

void* Tiny::work(void *args) {
    int efd = epoll_create1(0);
    if(efd == -1)
        return NULL;
    struct epoll_event *events = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));
    while(1){
        std::cout << "wait for new connection in " << efd << std::endl;
        socketQueue->remove();
        std::cout << "\nget a new connection in "<< efd <<" === " << socketQueue->size() << std::endl;
        notice_handler(efd);
        bool connected = true;
        while(connected){
            std::cout << "\nwait for next request in " << efd <<" === " << socketQueue->size() << std::endl;
            int n = epoll_wait(efd, events, MAXEVENTS, -1);
            for (int i = 0; i < n; i++){
                if ((events[i].events & EPOLLERR) ||
                    (events[i].events & EPOLLHUP) ||
                    (!(events[i].events & EPOLLIN))){
                    if(events[i].data.ptr!=NULL){
                        std::cout << "???" << std::endl;
                        SocketStream *socket = (SocketStream *)events[i].data.ptr;
                        delete socket;
                        connected = false;
                    }else{
                        logger::error <<"[Worker] " << efd << " " << events[i].data.ptr; 
                        std::cout << "ERROR at " << events[i].data.ptr << std::endl;
                    }
                    continue;
                }
                if( events[i].data.ptr!=NULL ){
                    int s = http_handler(events[i]);
                    connected = (s>0);
                }else{
                    std::cout << "OTHER ERROR" << std::endl;
                    connected = false;
                }
            }
            //std::cout << "efd: "<< efd << " "<< connected << std::endl;
        }
    }
    free(events);
    return NULL;
}

int Tiny::notice_handler(int efd){
    struct sockaddr_in clientaddr;
    static socklen_t clientlen = sizeof(clientaddr);
    int infd = SocketStream::Accept((struct sockaddr*)&clientaddr, &clientlen);
    if(infd>0){
         // attach SocketStream to notice.event.data.ptr
         SocketStream *socket = new SocketStream(infd,clientaddr,efd); // need to free
         std::cout << "accepted connection on fd " << infd 
                       << " by " << efd << std::endl;
    }
    return infd;
}

int Tiny::http_handler(const struct epoll_event& event){
    if(event.data.ptr==NULL)
        return -1;
    SocketStream *socket = (SocketStream *)event.data.ptr;
    HttpRequest *request = (HttpRequest *)socket->handling;
    if(request==NULL){
        request = new HttpRequest();
        socket->handling = request;
    }
    int s = parse(socket, request);
    std::cout << socket->fd << " s:" << s << " " << request->status << " "<< request->GetUri() << std::endl;
    if(s==0){
        //if(request->status == Ready){
            delete request;
            delete socket;
        //}
    }else if(s<0){
        // 非法请求
        logger::debug << "[Tiny] http_handler: parse return " << s << logger::endl;
        delete request;
        delete socket;
    }else{
        auto response = route(request);
        cout << "req: " << request->GetHeader("connection") << endl;
        s = HttpProtocol::ConnectionHandler(request,response);
        // s= 0[close] 1[keepalive]
        socket->reply_cnt++;
        if(socket->reply_cnt==1){
            response->SetHeader("connection","close");
            s = 0;
        }
        reply(socket, response);
        delete request;
        socket->handling = NULL;
        cout << "res: "<< s << " " <<  response->GetHeader("connection") << endl;
        //keepalive timeout 需要设置
        if(s==0)
            delete socket;
    }
    return s;
}
int Tiny::handle(const tiny_http_request_t* request, tiny_http_response_t* response){
    for (const auto &item : handler_list) {
        if (!tiny_router_locate(request,item))
            continue;
        if (item.handler(request, response) == TINY_SUCCESS) {
            item.count++;
            return TINY_SUCCESS;
        }
    }
}
int Tiny::reply(SocketStream *socket, shared_ptr<HttpResponse> response) {
    if(response==NULL){
        return -1;
    }
    logger::debug << "[Tiny::reply] " << response->GetBody().length();
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
		get(item.first,item.second);
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
        router->append(suffix, [&, folder, suffix](vector<string> args) {
            if(args.size() >= 1){
                string buf;
                string filename = folder + args[0];
                int n = Template::ReadFile(filename,buf);
                if(n>=0)
                    return new HttpResponse(buf,HttpMessage::GetType(suffix));
			}
            return router->Get404Response();
        }, true);
    }
    return *this;
}

// single uri
Tiny& Tiny::route(string uri, tiny_http_handler_pt h) {
    router->append(uri, h);
    return *this;
}
Tiny& Tiny::get(string uri, const make_response_function &callback) {
    router->append(uri, callback, false, "GET");
    return *this;
}

Tiny& Tiny::post(string uri, const make_response_function &callback) {
    router->append(uri, callback, false, "POST");
    return *this;
}


int Tiny::parse(SocketStream *socket, HttpRequest *request) {
    if(request == NULL || socket == NULL) 
        return -1;
    int s = HttpProtocol::ParseFirstLine(*socket, *request);
    if(s < 0) 
        return -2;
    s = HttpProtocol::ParseHeader(*socket, *request);
    if(s < 0)
        return -3;
    s = HttpProtocol::ParseBody(*socket, *request);
    if(s < 0)
        return -4;
    return s;	
}

}
