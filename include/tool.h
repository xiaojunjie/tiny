#pragma once

#include <semaphore.h>
#include <dlfcn.h>

void Sem_init(sem_t *sem, int pshared, unsigned int value);
void P(sem_t *sem);
void V(sem_t *sem);
void *Dlopen(const char *filename, int flag);
void *Dlsym(void *handle, char *symbol);
int Dlclose(void *handle);
