#pragma once

#include <memory>
#include "file_logstream.h"
namespace logger{
    typedef struct {
        Level type;
        //for first msg  .eg:
        //logger::debug << msg;
        template <typename T>
        std::shared_ptr<logstream> operator<<(const T& x){
            std::shared_ptr<logstream> p = std::make_shared<logstream>(type);
            *p << x;
            return p;
        }
        //for endl without msg  .eg:
        //logger::debug << logger::endl;
        std::shared_ptr<logstream> operator<<(end_fun manip){
            std::shared_ptr<logstream> p = std::make_shared<logstream>(type); 
            *p << manip;
            return p;
        }
    } LogBoot;

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
}
