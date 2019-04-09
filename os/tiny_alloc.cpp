#include "tiny_alloc.h"
#include "tiny_file.h"
namespace tiny {

void tiny_free(void* p){
    free(p);
}

void *tiny_alloc(tiny_int_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        logger::error << "[os] tiny_alloc: malloc() " << size << " bytes failed"
                      << logger::endl;
    }
    return p;
}

void* tiny_memalign(tiny_int_t alignment, tiny_int_t size){
    void *p;
    tiny_int_t err = posix_memalign(&p, alignment, size);
    if(err){
        logger::error << "posix_memalign("<< alignment <<", "<< size <<") failed";
        p = NULL;
    }
    return p;
}

}
