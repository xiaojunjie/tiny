#include "tiny_threadpool.h"

int ThreadPool::count() { return accumulate(tid, tid+THREAD_MAX, 0); }
int ThreadPool::start(handler_t func, void * _this) {
    for (int i = 0; i < THREAD_INIT; i++) {
        std::thread t(func, _this);
        t.detach();
        tid[i] = true;
    }
    return count();
}
