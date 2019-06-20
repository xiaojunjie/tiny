#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#define LISTENQ_G 4
#define LISTENQ_L 1024
namespace tiny{

template <class T>
class Sbuf{
    public:
        Sbuf();
        // ~Sbuf();
        int insert(const T &);
        T remove();
        unsigned int size() const;
    private:
        std::queue<T> queue;
        const unsigned int capacity;
        std::mutex mutex;
        std::condition_variable producer;
        std::condition_variable consumer;
    };

    template <class T>
    Sbuf<T>::Sbuf():capacity(LISTENQ_G){}

    template <class T>
    unsigned int Sbuf<T>::size() const{
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size(); 
    }
    template <class T>
    int Sbuf<T>::insert(const T & item){
        logger::debug << "prepare to insert 1 item into sbuf ... " << logger::endl;
        std::unique_lock<std::mutex> lock(mutex);
        producer.wait(lock,[=](){return queue.size()<capacity;});
        queue.push(item);
        consumer.notify_one();
        auto size = queue.size();
        lock.unlock();
        logger::debug << size <<" item in sbuf after insert" << logger::endl;
        return size;
    }

    template <class T>
    T Sbuf<T>::remove(){
        logger::debug << "prepare to get 1 item from sbuf ... " << logger::endl;
        T res = NULL;
        
        std::unique_lock<std::mutex> lock(mutex);
        consumer.wait(lock,[=](){return !queue.empty();});
        
        if(!queue.empty()){
            res = queue.front(); queue.pop();
        }
        producer.notify_one();
        auto size = queue.size();
        lock.unlock();
        logger::debug << size <<" item in sbuf after remove" << logger::endl;
        return res;
    }
}
