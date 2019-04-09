#include "tiny_base.h"
#include <cstdlib>
#define TINY_POOL_ALIGNMENT 16
#define TINY_MAX_ALLOC_FROM_POOL 4095
namespace tiny {
struct tiny_pool_large_t {
    tiny_pool_large_t *next;
    void *alloc;
};
struct tiny_pool_t {
    tiny_char_t *last;
    tiny_char_t *end;
    tiny_pool_t *current;
    tiny_pool_t *next;
    tiny_pool_large_t *large;
    tiny_int_t size;
    tiny_int_t max;
    tiny_int_t failed;
};

tiny_pool_t *tiny_create_pool(tiny_int_t size) {
    tiny_pool_t *p = tiny_memalign(TINY_POOL_ALIGNMENT, size);
    if (p == NULL) {
        return NULL:
    }
    p->last = (tiny_char_t *)p + sizeof(tiny_pool_t);
    p->end = (tiny_char_t *)p + size;
    p->next = NULL;
    p->max = MIN(p->end - p->last, TINY_MAX_ALLOC_FROM_POOL);
    p->current = p;
    p->failed = 0;
    return p;
}

static void *tiny_palloc_block(tiny_pool_t *pool, tiny_int_t size) {
    tiny_pool_t *p, *current;
    tiny_int_t psize = (tiny_int_t)(pool->end - (u_char *)pool);
    tiny_pool_t *newp = tiny_create_pool(psize);
    if (newp == NULL)
        return NULL;
    tiny_char_t *m = (tiny_char_t *)newp + sizeof(tiny_pool_t);
    newp->last = m + size;
    current = pool->current;
    for (p = current; p->next != NULL; p = p->next) {
        if (p->failed++ > 0)
            current = p->next;
    }
    p->next = newp;
    return m;
}

void tiny_destroy_pool(tiny_pool_t *pool) {
    for (tiny_pool_large_t *l = pool->large; l; l = l->next) {
        if (l->alloc != NULL) {
            ngx_free(l->alloc);
        }
    }
    for (tiny_pool_t *p = pool; p != NULL; p = p->next) {
        tiny_free(p);
    }
}
}
