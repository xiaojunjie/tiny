#include "file_logstream.h"
#include <mutex>
#include <ctime>
static std::string  Get_Current_Date(){
    char tmp[16];
    time_t nowtime = time(NULL);
    strftime(tmp,sizeof(tmp),"%Y-%m-%d",localtime(&nowtime));
    return tmp;
}
namespace logger{
    std::ofstream logstream::logfile;
    Level logstream::level;
    std::mutex logstream::log_mutex;
    std::mutex logstream::out_mutex;
    logstream debug(Debug);
    logstream info(Info);
    logstream warm(Warm);
    logstream error(Error);
    logstream fatal(Fatal);
    static logstream debug_next(Debug);
    static logstream info_next(Info);
    static logstream warm_next(Warm);
    static logstream error_next(Error);
    static logstream fatal_next(Fatal);
    static void bind(logstream& a, logstream& b){
        a.next = &b;
        b.prev = &a;
        a.prev = NULL;
        b.next = NULL;
    }
    void init(std::string filefolder, Level l){
        logstream::level = l;
        std::string filename = filefolder + Get_Current_Date() + ".txt";
        logstream::logfile.open(filename, std::ofstream::out | std::ofstream::trunc );
        bind(debug,debug_next);
        bind(info,info_next);
        bind(warm,warm_next);
        bind(error,error_next);
        bind(fatal,fatal_next);
    }
    void destroy() {
        logstream::logfile.close();
    }
    logstream& logstream::operator<<(end_fun manip){
        if( loglevel < level ){
            return *this;
        }
        logstream& p = manip(*this);
        if(this->next == NULL)
            log_mutex.unlock();
        return p;
    }
}
