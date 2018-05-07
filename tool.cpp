#include "tool.h"
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdexcept>

void Sem_init(sem_t *sem, int pshared, unsigned int value){
    if (sem_init(sem, pshared, value) < 0)
        throw std::runtime_error(strerror(errno));
}
void P(sem_t *sem){
    if (sem_wait(sem) < 0)
        throw std::runtime_error(strerror(errno));
}
void V(sem_t *sem){
    if (sem_post(sem) < 0)
        throw std::runtime_error(strerror(errno)); 
}
void *Dlopen(const char *filename, int flag){
    void *handle = dlopen(filename,flag);
    if (!handle) {
        throw std::runtime_error(strerror(errno)); 
    }
    return handle;
}

void *Dlsym(void *handle, char *symbol) {
    char *error;
    void *func = dlsym(handle, symbol);
    if ((error = dlerror()) != NULL) {
        throw std::runtime_error(strerror(errno)); 
    }
    return func;
}

int Dlclose(void *handle){
    int result;
    if ((result=dlclose(handle)) < 0) {
        throw std::runtime_error(strerror(errno)); 
    }
    return result;
}
