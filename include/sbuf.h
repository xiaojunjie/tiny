#pragma once

#include "tool.h"
#include <queue>
#include <mutex>
#define LISTENQ_G 4
#define LISTENQ_L 1024
namespace tiny{
    template <class T>
    class Sbuf{
    public:
        Sbuf();
        // ~Sbuf();
        int insert(T*);
        T* remove();
        int size();
    private:
        std::queue<T*> queue;
        const int n;             /* Maximum number of slots */
        std::mutex q_mutex;       /* Protects accesses to buf */
        sem_t slots;       /* Counts available slots */
        sem_t items;       /* Counts available items */
    };

    template <class T>
    Sbuf<T>::Sbuf():n(LISTENQ_G){
        Sem_init(&slots, 0, n);      /* Initially, buf has n empty slots */
        Sem_init(&items, 0, 0);      /* Initially, buf has zero data items */
    }

    template <class T>
    int Sbuf<T>::size(){
        q_mutex.lock();
        int count = queue.size(); 
        q_mutex.unlock();
        return count;
    }
    template <class T>
    int Sbuf<T>::insert(T* item){
        logger::debug << "prepare to insert 1 item into sbuf ... " << logger::endl;
        P(&slots);                          /* Wait for available slot */
        q_mutex.lock();
        queue.push(item);
        int count = queue.size();
        q_mutex.unlock();
        V(&items);                          /* Announce available item */
        logger::debug << count <<" item in sbuf after insert" << logger::endl;
        return count;
    }

    template <class T>
    T* Sbuf<T>::remove(){
        logger::debug << "prepare to get 1 item from sbuf ... " << logger::endl;
        T *p;
        int count = 0;
        P(&items);                          /* Wait for available item */
        q_mutex.lock();
        if(queue.empty()){
            p = NULL;
        }else{
            p = queue.front();
            queue.pop();
            count = queue.size();
        }
        q_mutex.unlock();
        V(&slots);                          /* Announce available slot */
        logger::debug << count <<" item in sbuf after remove" << logger::endl;
        return p;
    }
}
