#include "file_logstream.h"
#include "file_logboot.h"
#include <mutex>
#include <ctime>
#include <queue>
#include <thread>
#include "tool.h"
#include <unistd.h>
#include <ios>

static std::string  Get_Current_Date(){
    char tmp[16];
    time_t nowtime = time(NULL);
    strftime(tmp,sizeof(tmp),"%Y-%m-%d",localtime(&nowtime));
    return tmp;
}
namespace logger{
    std::ofstream logstream::logfile;
    std::queue<std::string> logstream::msgQueue;
    Level logstream::level;
    std::mutex logstream::log_mutex;
    sem_t logstream::sem_msg;
    LogBoot debug = {Debug};
    LogBoot info = {Info};
    LogBoot warm = {Warm};
    LogBoot error = {Error};
    LogBoot fatal = {Fatal};
    static void writeToFile(){
        while(1){
            P(&logstream::sem_msg);    /* Wait for  msg */
            logstream::log_mutex.lock();
            std::string msg = logstream::msgQueue.front();
            logstream::msgQueue.pop();
            logstream::log_mutex.unlock();
            logstream::logfile << msg;
            logstream::logfile.flush();
        }
    }
    void init(std::string filefolder, Level l){
        logstream::level = l;
        std::string filename = filefolder + Get_Current_Date() + ".txt";
        logstream::logfile.open(filename, std::ofstream::out | std::ofstream::trunc );
        Sem_init(&logstream::sem_msg, 0, 0);
        std::thread writer(&writeToFile);
        writer.detach();
    }
    void destroy() {
        while(1){
            logstream::log_mutex.lock();
            int size = logstream::msgQueue.size();
            logstream::log_mutex.unlock();
            if(size>0)
                usleep(100000); // wait for writer
            else
                break;
        }
        logstream::logfile.close();
    }

    logstream::~logstream(){
        msgBuf.seekp(0, std::ios::end);
        if(msgBuf.tellp()>0){
            dump();
        }
    }

    void logstream::dump(){
        if( type >= level   ){
            msgBuf << "\n";
            log_mutex.lock();
            msgQueue.push(msgBuf.str());
            log_mutex.unlock();
            V(&sem_msg);
            msgBuf.clear();
            msgBuf.str("");
        }
    }

    logstream& logstream::operator<<(end_fun manip){
        if( type >= level  ){
            manip(*this);
        }
        return *this;
    }
    std::shared_ptr<logstream> operator<<(std::shared_ptr<logstream> log, end_fun manip){
        *log << manip;
        return log;
    }
}
