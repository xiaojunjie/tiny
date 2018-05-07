#pragma once

#include "tool.h"
#include <vector>
#include "file.h"
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
    private:
        std::vector<T*> queue;
        const int n;             /* Maximum number of slots */
        sem_t mutex;       /* Protects accesses to buf */
        sem_t slots;       /* Counts available slots */
        sem_t items;       /* Counts available items */
    };

    template <class T>
    Sbuf<T>::Sbuf():n(LISTENQ_G){
        Sem_init(&mutex, 0, 1);      /* Binary semaphore for locking */
        Sem_init(&slots, 0, n);      /* Initially, buf has n empty slots */
        Sem_init(&items, 0, 0);      /* Initially, buf has zero data items */
    }

    template <class T>
    int Sbuf<T>::insert(T* item){
        logger::debug << "prepare to insert 1 item into sbuf ... " << logger::endl;
        P(&slots);                          /* Wait for available slot */
        P(&mutex);                          /* Lock the buffer */
        queue.push_back(item);
        int count = queue.size();
        V(&mutex);                          /* Unlock the buffer */
        V(&items);                          /* Announce available item */
        logger::debug << count <<" item in sbuf after insert" << logger::endl;
        return count;
    }

    template <class T>
    T* Sbuf<T>::remove(){
        logger::debug << "prepare to get 1 item from sbuf ... " << logger::endl;
        P(&items);                          /* Wait for available item */
        P(&mutex);                          /* Lock the buffer */
        T* p = queue[0];
        queue.erase(queue.begin());
        int count = queue.size();
        V(&mutex);                          /* Unlock the buffer */
        V(&slots);                          /* Announce available slot */
        logger::debug << count <<" item in sbuf after remove" << logger::endl;
        return p;
    }
}
