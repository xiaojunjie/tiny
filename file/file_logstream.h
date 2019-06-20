#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <mutex>
#include <queue>
#include <condition_variable>
namespace logger{
    class logstream;
    struct logboot;
    typedef logstream& (*end_fun)(logstream&);
    typedef enum {Debug, Info, Warm, Error, Fatal} Level;
    struct logstream{
        logstream(Level l):type(l){}
        ~logstream();
        std::ostringstream msgBuf;
        static Level level; // Log Level of Server

        template <typename T>
        logstream& operator<<(const T&);
        logstream& operator<<(end_fun);
        const Level type; // current Log Level of logstream
        void dump();
    };
    
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
    
    struct logboot{
        Level type;
        
        //for first msg  .eg:
        //logger::debug << msg;
        template <typename T>
        std::shared_ptr<logstream> operator<<(const T& x);

        //for endl without msg  .eg:
        //logger::debug << logger::endl;
        std::shared_ptr<logstream> operator<<(end_fun manip);
    };
    template <typename T> 
    std::shared_ptr<logstream> logboot::operator<<(const T& x){
        std::shared_ptr<logstream> p = std::make_shared<logstream>(type);
        *p << x;
        return p;
    }

    //for Non-first msg  .eg:
    //logger::debug << ... << msg;
    template <typename T>
    std::shared_ptr<logstream> operator<<(std::shared_ptr<logstream> log, const T& s){
        if( log->type >= logstream::level )
            log->msgBuf << s;
        return log;
    }
    // for endl with msg  .eg:
    //logger::debug << ... << logger::endl;
    std::shared_ptr<logstream> operator<<(std::shared_ptr<logstream>, end_fun);
    
    template <typename T>
    T& endl(T& stream){
        stream.dump();
        return stream;
    }
    
}
