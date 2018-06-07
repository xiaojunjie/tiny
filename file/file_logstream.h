#pragma once

#include <sstream>
#include <iostream>
#include <fstream>
#include <mutex>
#include <queue>
#include "tool.h"
namespace logger{
    typedef enum {Debug, Info, Warm, Error, Fatal} Level;
    class logstream{
    public:
        logstream(Level l):loglevel(l){}
        ~logstream();
        std::ostringstream msgBuf;
        static sem_t sem_msg;
        static std::mutex log_mutex;
        static std::ofstream logfile;
        static std::queue<std::string> msgQueue;
        static Level level;

        template <typename T>
        logstream& operator<<(const T&);

        typedef logstream& (*end_fun)(logstream&);
        logstream& operator<<(end_fun);
        logstream *prev;
        logstream *next;
        Level loglevel;
        void dump();
    };
    typedef logstream& (*end_fun)(logstream&);

    template <typename T>
    logstream& logstream::operator<<(const T& x){
        const static char* levelmsg[] = {"Debug","Info","Warm","Error","Fatal"};
        std::string buf;
        time_t now = time(NULL);
        char tmp[24];
        strftime(tmp,sizeof(tmp),"%F %X",localtime(&now));
        buf += "[";
        buf += tmp;
        buf += "] ";
        buf += levelmsg[loglevel];
        buf += ": ";
        this->msgBuf << buf << x;
        return *this;
    }
    
    template <typename T>
    T& endl(T& stream){
        stream.dump();
        return stream;
    }
}
