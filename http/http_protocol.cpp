#include "http_protocol.h"
#include <vector>
#include <map>
#include <algorithm>  
#include "file.h"
#include <iostream>
namespace tiny{
    static std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        if(s.back() == delim)
            elems.push_back(string());
        return elems;
    }
    int HttpProtocol::ParseFirstLine(SocketStream &socket, HttpRequest& request){
        std::string buf = socket.readline();
        std::vector<std::string> header = split(std::string(buf), ' ');
        if(header.size()==3){
			std::transform(header[1].begin(), header[1].end(), header[1].begin(), ::tolower);
            request.SetMethod(header[0]);
            request.SetUri(header[1]);
            request.SetVersion(header[2]);
        }else{
            logger::info << "[HttpProtocol] ParseFirstLine Error " << request << logger::endl;
            return -1;
        }
        const char* pos = strrchr(header[1].c_str(),'?');
        if(pos==NULL)return 1;
        string str(pos+1);
        string path = header[1].substr(0,header[1].length()-strlen(pos));
        request.SetUri(path);
        for(const auto &item: split(str,'&')){
            vector<string> kv = split(item,'=');
            if(kv.size()==2){
                request.SetQuery(kv[0],kv[1]);
            }
        }
        return 1;
    }
    int HttpProtocol::ParseHeader(SocketStream &socket, HttpRequest& request){
        char buf[MAXLINE];
        const static int MaxHeaderSize = 64;
        int i;
        for(i=0; i < MaxHeaderSize; i++) {
			int n = socket.readline(buf, MAXLINE);
            if( n <= 0 || strcmp(buf,"\r\n") == 0){
                break;
			}else{
				std::transform(buf, buf+n, buf, ::tolower);
			}
            char *p;
            p = strchr(buf, '\r');
            if( p && p < buf+n )
                *p = '\0';
            else
                return -1;
            p = strchr(buf, ':');
            if(p && p < buf+n)
                *p = '\0';
            else
                return -1;
            if(strlen(buf)>0 && strlen(p+2)>0){
                string key(buf);
                string value(p+2);
                request.SetHeader(key,value);
            }else{
                return -1;
            }
        }
        return i;
    }

    int HttpProtocol::ParseBody(SocketStream &socket, HttpRequest& request){
		if( request.GetMethod().compare("POST") !=0 )
			return 0;
		int content_length = atoi( request.GetHeader(HttpMessage::HEADER_CONTENT_LENGTH).c_str() );
		if (content_length > 0){
			int buf_size = MIN(content_length, BUFSIZE);
			char *buf = new char[buf_size+1];
			int n = socket.readn(buf,buf_size);
			buf[buf_size] = '\0';
			if(n > 0)
				request.SetBody(buf);
			delete buf;
			return n;
		}else if(!request.GetHeader(HttpMessage::HEADER_TRANSFER_ENCODING).empty()){
			string firstline = socket.readline();
			int buf_size = strtol(firstline.c_str(),NULL,16);
			buf_size = MIN(buf_size, BUFSIZE*MAXLINE);
			char *buf = new char[buf_size+1];
			int n = socket.readn(buf,buf_size);
			buf[buf_size] = '\0';
			if(n > 0)
				request.SetBody(buf);
			delete buf;
			return n;
		}else{
			return 0;
		}
    }

    int HttpProtocol::SendMessage(SocketStream &socket, const HttpResponse &response){
        ostringstream html;
        html << response;
        return socket.writen(html.str());
    }
}
