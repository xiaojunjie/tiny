#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <mutex>
#include <queue>
#include <condition_variable>
namespace logger{
    typedef enum {Debug, Info, Warm, Error, Fatal} Level;
    class logstream{
    public:
        logstream(Level l):type(l){}
        ~logstream();
        std::ostringstream msgBuf;
        static std::condition_variable writer;
        static std::mutex mutex;
        static std::ofstream logfile;
        static std::queue<std::string> msgQueue;
        static Level level; // Log Level of Server

        template <typename T>
        logstream& operator<<(const T&);

        typedef logstream& (*end_fun)(logstream&);
        logstream& operator<<(end_fun);
        const Level type; // current Log Level of logstream
        void dump();
    };
    typedef logstream& (*end_fun)(logstream&);

    template <typename T>
    logstream& logstream::operator<<(const T& x){
        if( type >= level   ){
            const static char* levelmsg[] = {"Debug","Info","Warm","Error","Fatal"};
            std::string buf;
            time_t now = time(NULL);
            char tmp[24];
            strftime(tmp,sizeof(tmp),"%F %X",localtime(&now));
            buf += "[";
            buf += tmp;
            buf += "] ";
            buf += levelmsg[type];
            buf += ": ";
            this->msgBuf << buf << x;
        }
        return *this;
    }
    
    template <typename T>
    T& endl(T& stream){
        stream.dump();
        return stream;
    }
}
