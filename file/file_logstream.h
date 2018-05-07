#pragma once

#include <fstream>
#include <mutex>
namespace logger{
    typedef enum {Debug, Info, Warm, Error, Fatal} Level;
    const static char* levelmsg[] = {"Debug","Info","Warm","Error","Fatal"};
    class logstream{
    public:
        static std::mutex out_mutex;
        static std::mutex log_mutex;
        logstream(Level l):loglevel(l){}
        static std::ofstream logfile;
        static Level level;

        template <typename T>
        logstream& operator<<(const T&);

        typedef logstream& (*end_fun)(logstream&);
        logstream& operator<<(end_fun);
        logstream *prev;
        logstream *next;
        Level loglevel;
        template <typename T>
        static void echo(const T& x);
    };

    template <typename T>
    logstream& logstream::operator<<(const T& x){
        if( loglevel < level ){
            return *this;
        }else if(this->next == NULL){
            echo(x);
            return *this;
        }else{
            std::string buf;
            time_t now = time(NULL);
            char tmp[24];
            strftime(tmp,sizeof(tmp),"%F %X",localtime(&now));
            buf += "[";
            buf += tmp;
            buf += "] ";
            buf += levelmsg[loglevel];
            buf += ": ";
            log_mutex.lock(); // lock
            echo(buf);
            echo(x);
            return *(this->next);
        }
    }

    template <typename T>
    void logstream::echo(const T& x){
        out_mutex.lock();
        logfile << x;
        out_mutex.unlock();
    }

    template <typename T>
    T& endl(T& stream){
        logstream::echo("\n");
        logstream::logfile.flush();
        if(stream.prev == NULL)
            return stream;
        else
            return *stream.prev;
    };
}
