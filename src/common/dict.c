#include <stdlib.h>
#include <stdio.h>

#include "./dict.h"


#define ALPHA (2)


int dict_hash(dict_t* d, uint64_t key);

dict_t* dict_resize(dict_t* d);

dict_t* dict_new(int len) {

    dict_t* d = malloc(sizeof(dict_t));

    d->len = len;
    d->free = len;

    d->key_val_ptr = calloc(d->len, sizeof(d->key_val));

    return d;
}

void dict_insert(dict_t** d, uint64_t key, uint64_t val) {

    if ((*d)->free <= (*d)->len/80) {
        *d = dict_resize(*d);
    }

    int index = dict_hash(*d, key);

    while ((*d)->key_val_ptr[index].key != 0) {
        index += 1;
        index %= (*d)->len;
    }

    (*d)->key_val_ptr[index].key = key;
    (*d)->key_val_ptr[index].val = val;

    (*d)->free -= 1;
}

uint64_t dict_search(dict_t* d, uint64_t key) {

    int index = dict_hash(d, key);

    int i = 0;
    while ((d->key_val_ptr[index].key != key) && (i < d->len)) {
        index += 1;
        index %= d->len;
        i += 1;
    }

    if (d->key_val_ptr[index].key == key) {
        return d->key_val_ptr[index].val;
    }
    return 0;
}

void dict_remove(dict_t* d, uint64_t key) {

    int index = dict_hash(d, key);

    int i = 0;
    while ((d->key_val_ptr[index].key != key) && (i < d->len)) {
        index += 1;
        index %= d->len;
        i += 1;
    }

    if (d->key_val_ptr[index].key == key) {
        d->key_val_ptr[index].key = 0;
        d->key_val_ptr[index].val = 0;

        d->free += 1;
    }
}

void dict_free(dict_t** d) {
    free((*d)->key_val_ptr);
    free((*d));
    *d = NULL;
}

int dict_hash(dict_t* d, uint64_t key) {
    return key % d->len;
}

dict_t* dict_resize(dict_t* d) {

    dict_t* d_new = dict_new(d->len*ALPHA);

    for (int i = 0; i < d->len; i++) {
        if (d->key_val_ptr[i].key != 0) {
            dict_insert(&d_new, d->key_val_ptr[i].key, d->key_val_ptr[i].val);
        }
    }

    dict_free(&d);

    return d_new;
}
