#include "tiny.h"
#include <sstream>
#include <regex>  
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <sys/eventfd.h>

namespace tiny {

const string Tiny::version = "1.0";
vector<int> Tiny::noticefds;

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
        HttpResponse *response = new HttpResponse(tpl.render(), 404);
        response->SetHeader(HttpMessage::HEADER_CONNECTION,"close");
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
        //int len = socketQueue->insert(socket);
        for(const auto& nfd: noticefds){
            //std::cout << "noticefds: " << nfd << std::endl;
            static uint64_t temp = 1; // temp>0
            if( write(nfd,&temp,sizeof(temp))<0 ){
                logger::warm << "[Tiny] notice error";
            }
        }
        //if(len >= LISTENQ_G) {
        //    //worker->extend(this);
        //} else if(len <= 1) {
        //    //worker->halve<Sbuf<SocketStream>>(socketQueue);
        //    //router->sort();
        //}
    });
}

int Tiny::add_noticefd(int fd){
    static std::mutex mtx;
    mtx.lock();
    noticefds.push_back(fd);
    mtx.unlock();
    return 1;
}

void* Tiny::work(void *args) {
    int efd = epoll_create1(0);
    int noticefd = eventfd(0,EFD_NONBLOCK);
    if(efd == -1 || noticefd == -1 || SocketStream::bind_noticefd(efd,noticefd) == -1)
        return NULL;
    add_noticefd(noticefd);

    struct epoll_event *events = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));
    std::thread::id id = std::this_thread::get_id();
    std::cout << "wait for notice of " << efd << " at thread " << id << std::endl;
    while(1){
        int n = epoll_wait(efd, events, MAXEVENTS, -1);
        for (int i = 0; i < n; i++){
            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))){
                if(events[i].data.fd != noticefd && events[i].data.ptr!=NULL){
                    SocketStream *socket = (SocketStream *)events[i].data.ptr;
                    //std::cout << "close " << socket->fd << " reply "<< socket->reply_cnt << std::endl;
                    delete socket;
                }else{
                    logger::error <<"[Worker] " << id << " " << events[i].data.ptr; 
                    //std::cout << "ERROR at " << events[i].data.ptr << std::endl;
                }
                continue;
            }
            if(events[i].data.fd == noticefd){
                notice_handler(events[i],efd);
            }else if( events[i].data.ptr!=NULL ){
                int s = http_handler(events[i]);
                //状态
            }else{
                std::cout << "OTHER ERROR" << std::endl;
            }
        }
    }
    free(events);
    return NULL;

}

int Tiny::notice_handler(const struct epoll_event& event, int efd){
    struct sockaddr_in clientaddr;
    static socklen_t clientlen = sizeof(clientaddr);
    int infd = SocketStream::Accept((struct sockaddr*)&clientaddr, &clientlen);
    if(infd>0){
         SocketStream *socket = new SocketStream(infd,clientaddr); // need to free
         SocketStream::add_connect(efd,infd,socket);
         logger::debug << "[socket] accepted connection on fd " << infd 
                       << " by " << std::this_thread::get_id();
    }
    return infd;
}

int Tiny::http_handler(const struct epoll_event& event){
    if(event.data.ptr==NULL)
        return -1;
    SocketStream *socket = (SocketStream *)event.data.ptr;
    HttpRequest *request = new HttpRequest();
    int s = parse(socket, request);
    if(s<0){
        // 非法请求
        logger::debug << "[Tiny] http_handler: parse return " << s << logger::endl;
        delete socket;
    }else{
        auto response = route(request);
        s = HttpProtocol::ConnectionHandler(request,response);
        reply(socket, response);
        socket->reply_cnt++;
    }
    delete request;
    return s;
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
    return 1;	
}

}
