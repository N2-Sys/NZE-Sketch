#ifndef _BLOOM_FILTER_H
#define _BLOOM_FILTER_H

#include "common.h"

template<uint32_t w, int hash_num> 
class BloomFilter{
    uint32_t size;
    uint32_t width;
    uint8_t *bit_array;
    uint64_t h[hash_num], s[hash_num], n[hash_num];

public:
    BloomFilter();
    ~BloomFilter();
    bool getbit(Key_t k);
    void setbit(Key_t k);
    void reset();
    size_t get_memory_usage();
    int get_hash_num();
};

template<uint32_t w, int hash_num>
BloomFilter<w, hash_num>::BloomFilter() {
    width = calNextPrime(w);
    size = (width >> 3) + ((width & 0x7) != 0);
    int index = 0;
    for (int i = 0; i < hash_num; ++i) {
        h[i] = GenHashSeed(index++);
        s[i] = GenHashSeed(index++);
        n[i] = GenHashSeed(index++);
    }
    bit_array = new uint8_t[size]();
}

template<uint32_t w, int hash_num>
BloomFilter<w, hash_num>::~BloomFilter() {
    delete []bit_array;
}

template<uint32_t w, int hash_num>
bool BloomFilter<w, hash_num>::getbit(Key_t k) {
    for (int i = 0; i < hash_num; ++i) {
        uint32_t pos = (uint32_t)(AwareHash((unsigned char *)k, KEY_T_SIZE, h[i], s[i], n[i]) % width);
        if (!(bit_array[pos >> 3] & (1 << (pos & 0x7))))
            return false;
    }
    return true;
}

template<uint32_t w, int hash_num>
void BloomFilter<w, hash_num>::setbit(Key_t k) {
    for (int i = 0; i < hash_num; ++i) {
        uint32_t pos = (uint32_t)(AwareHash((unsigned char *)k, KEY_T_SIZE, h[i], s[i], n[i]) % width);
        bit_array[pos >> 3] |= (1 << (pos & 0x7));
    }
}

template<uint32_t w, int hash_num>
void BloomFilter<w, hash_num>::reset() {
    memset(bit_array, 0, size);
}

template<uint32_t w, int hash_num>
size_t BloomFilter<w, hash_num>::get_memory_usage() {
    return size;
}

template<uint32_t w, int hash_num>
int BloomFilter<w, hash_num>::get_hash_num() {
    return hash_num;
}

#endif
