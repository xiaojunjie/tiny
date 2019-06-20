#include "file_logstream.h"
#include "file_logboot.h"
#include <ctime>
#include <thread>
#include <unistd.h>
#include <ios>

static std::string  get_current_date(){
    char tmp[16];
    time_t nowtime = time(NULL);
    strftime(tmp,sizeof(tmp),"%Y-%m-%d",localtime(&nowtime));
    return tmp;
}
namespace logger{
    std::ofstream logstream::logfile;
    std::queue<std::string> logstream::msgQueue;
    Level logstream::level;
    std::mutex logstream::mutex;
    std::condition_variable logstream::writer;
    LogBoot debug = {Debug};
    LogBoot info = {Info};
    LogBoot warm = {Warm};
    LogBoot error = {Error};
    LogBoot fatal = {Fatal};
    void init(std::string filefolder, Level l){
        logstream::level = l;
        std::string filename = filefolder + get_current_date() + ".txt";
        logstream::logfile.open(filename, std::ofstream::out | std::ofstream::trunc );
        std::thread writer([](){
            while(1){
                std::unique_lock<std::mutex> lock(logstream::mutex);
                logstream::writer.wait(lock,[=](){return !logstream::msgQueue.empty();});
                std::string msg = logstream::msgQueue.front();
                logstream::msgQueue.pop();
                lock.unlock();
                logstream::logfile << msg;
                logstream::logfile.flush();
            }
        });
        writer.detach();
    }
    void destroy() {
        while(logstream::msgQueue.size()>0) usleep(100000); // wait for writer
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
            std::lock_guard<std::mutex> lock(mutex);
            msgQueue.push(msgBuf.str());
            writer.notify_one();
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
