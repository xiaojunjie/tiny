#pragma once

#include "http_message.h"
#include "socket.h"
#include <sstream>

namespace tiny{
    class HttpProtocol{
    public:
        static int ParseFirstLine(SocketStream &, HttpRequest &);
        static int ParseHeader(SocketStream &, HttpRequest &);
        static int ParseBody(SocketStream &, HttpRequest &);
        static int SendMessage(SocketStream &, const HttpResponse &);
    };
    // template<typename Out>
    // void split(const std::string &s, char delim, Out result) {
    //     std::stringstream ss(s);
    //     std::string item;
    //     while (std::getline(ss, item, delim)) {
    //         *(result++) = item;
    //     }
    // }
}
