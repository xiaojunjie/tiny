#pragma once
#include <functional>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

#define THREAD_INIT 20
#define THREAD_MAX 128

using namespace std;

class ThreadPool {
  public:
    typedef function<void*(void*)> handler_t;
    int start(handler_t, void*);
    int count();

  private:
    bool tid[THREAD_MAX] = {0};
    mutex mtx; // /* Protects accesses to tid */
};

