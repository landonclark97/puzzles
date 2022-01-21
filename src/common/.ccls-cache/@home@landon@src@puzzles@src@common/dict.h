#ifndef __DICT__
#define __DICT__


#include <stdint.h>


#define INIT_DICT_LEN (1000)


typedef struct {
    int len, free;
    struct {
        uint64_t key, val;
    } key_val, *key_val_ptr;
} dict_t;


#define DICT_NEW() dict_new(INIT_DICT_LEN)

dict_t* dict_new(int len);

void dict_insert(dict_t** d, uint64_t key, uint64_t val);

uint64_t dict_search(dict_t* d, uint64_t key);

void dict_remove(dict_t* d, uint64_t key);

void dict_free(dict_t** d);









#endif
