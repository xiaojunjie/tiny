#include "file_logstream.h"
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
    Level logstream::level;
    logboot debug = {Debug};
    logboot info = {Info};
    logboot warm = {Warm};
    logboot error = {Error};
    logboot fatal = {Fatal};

    static std::condition_variable writer;
    static std::mutex mutex;
    static std::ofstream logfile;
    static std::queue<std::string> msgQueue;
    void init(std::string filefolder, Level l){
        logstream::level = l;
        std::string filename = filefolder + get_current_date() + ".txt";
        logfile.open(filename, std::ofstream::out | std::ofstream::trunc );
        std::thread writer_t([](){
            while(1){
                std::unique_lock<std::mutex> lock(mutex);
                writer.wait(lock,[=](){return !msgQueue.empty();});
                std::string msg = msgQueue.front();
                msgQueue.pop();
                lock.unlock();
                logfile << msg;
                logfile.flush();
            }
        });
        writer_t.detach();
    }
    void destroy() {
        while(msgQueue.size()>0) usleep(100000); // wait for writer
        logfile.close();
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
    std::shared_ptr<logstream> logboot::operator<<(end_fun manip){
        std::shared_ptr<logstream> p = std::make_shared<logstream>(type); 
        *p << manip;
        return p;
    }
}
