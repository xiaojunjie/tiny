#pragma once

#define THREAD_INIT 1
#define THREAD_MAX 128
#include <mutex>
#include <thread>
#include "file.h"
using namespace std;
template<class T> class ThreadPool{
public:
    ThreadPool(T *);
    ~ThreadPool();
    // void init(T *);
    int extend(T *);
    template<class Q> int halve(Q*);
    int conut();
    int remove(bool *);
private:
    bool tid[THREAD_MAX];
    mutex mtx; // /* Protects accesses to tid */
};

template<class T>
int ThreadPool<T>::conut(){
    int i,j;
    mtx.lock();
    for (i = 0,j = 0; i < THREAD_MAX; i++){
        j += tid[i];
    }
    mtx.unlock();
    return j;
}

template<class T>
ThreadPool<T>::ThreadPool(T *p){
    for (int i = 0; i < THREAD_INIT; i++) {
        std::thread t(&T::work,p,tid+i);
        t.detach();
        tid[i] = true;
    }
    for (int i = THREAD_INIT; i < THREAD_MAX; i++) {
        tid[i] = false;
    }
}

template<class T>
ThreadPool<T>::~ThreadPool(){}

// template<class T>
// void ThreadPool<T>::init(T *p){
//     for (int i = 0; i < THREAD_INIT; i++) {
//         std::thread t(&T::work,p,tid+i);
//         t.detach();
//         tid[i] = true;
//     }
//     for (int i = THREAD_INIT; i < THREAD_MAX; i++) {
//         tid[i] = false;
//     }
// }

template<class T>
int ThreadPool<T>::extend(T *p){
    int count = conut();
    int add = MIN(count,THREAD_MAX-count);
    int i,j;
    mtx.lock();
    for (i=0, j=0; i < THREAD_MAX && j<add; i++) {
        if(!tid[i]){
            std::thread t(&T::work,p,tid+i);
            t.detach();
            tid[i] = true;
            j++;
        }
    }
    mtx.unlock();
    logger::debug << "thread extend to " << count+j << logger::endl;
    return count+j;
}

template<class T>
template<class Q>
int ThreadPool<T>::halve(Q* s){
    int count = conut();
    logger::debug << count/2 <<" thread need to exit" << logger::endl;
    for (int i = count/2; i > 0; i--) {
        s->insert(NULL);
    }
    return (count+1)/2;
}

template<class T>
int ThreadPool<T>::remove(bool *p){
    logger::debug << "1 thread is exiting ... " << logger::endl;
    mtx.lock();
    if(p)
        *p = false;
    mtx.unlock();
    int n = conut();
    if(p)
        logger::debug <<" 1 thread exit, " << n << " remain." << logger::endl;
    return n;
}
