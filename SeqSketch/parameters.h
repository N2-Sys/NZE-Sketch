#ifndef _PARAMETERS_H
#define _PARAMETERS_H

//parameters of CM sketch
#define CM_DEPTH 4
#define CM_WIDTH (1 << 14)

//parameters of Bloom Filter
#define BLOOM_SIZE (1 << 20)
#define BLOOM_HASH_NUM 9

//parameters of hash table
#define SLOT_NUM (1 << 15)
#define EVICT_THRESHOLD 1

//return value of hash table's insertion
#define HIT 0
#define MISS_EVICT 1
#define MISS_INSERT 2

#endif