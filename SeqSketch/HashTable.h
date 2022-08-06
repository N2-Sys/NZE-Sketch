#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include "common.h"
#include "parameters.h"

template<typename T>
class slot {
public:
	trace_t key;
	T val;
	T negativeCounter;

	slot(trace_t k, T v = 0):key(k), val(v) {
		negativeCounter = 0;
	}

	slot(Key_t k) {
		key = trace_t(k);
		val = negativeCounter = 0;
	}

	slot() {
		val = negativeCounter = 0;
	}

	static size_t get_memory_usage() {
		// return 2 * sizeof(T) + KEY_T_SIZE;
		return 2 * sizeof(T) + 2;
	}
};

template<typename T, uint32_t slot_num>
class hashTable {
public:
	uint32_t size;
	slot<T> *slots;
	uint64_t h, s, n;

	hashTable() {
		size = calNextPrime(slot_num);
		slots = new slot<T>[size];
		int index = 0;
		h = GenHashSeed(index++);
		s = GenHashSeed(index++);
		n = GenHashSeed(index++);
	}

	~hashTable() {
		delete []slots;
	}

	int insert(slot<T> &tempSlot, Key_t key, T val = 1) {
		trace_t tempKey = trace_t(key);
		uint32_t pos = (uint32_t)(AwareHash((unsigned char *)key, KEY_T_SIZE, h, s, n) % size);
		
		if (slots[pos].key == tempKey) {
			slots[pos].val += 1;
			return HIT;
		}
		else if (slots[pos].key.isEmpty()) {
			slots[pos].val = 1;
			slots[pos].key = tempKey;
			return HIT;
		}
		else {
			slots[pos].negativeCounter += 1;
			if (slots[pos].negativeCounter / slots[pos].val >= EVICT_THRESHOLD) {
				tempSlot = slots[pos];
				slots[pos] = slot<T>(tempKey, val);
				return MISS_EVICT;
			}
			else {
				tempSlot = slot<T>(tempKey, val);
			}
		}

		return MISS_INSERT;
	}

	T query(Key_t key) {
		trace_t tempKey = trace_t(key);
		uint32_t pos = (uint32_t)(AwareHash((unsigned char *)key, KEY_T_SIZE, h, s, n) % size);

		if (slots[pos].key == tempKey) {
			return slots[pos].val;
		}

		return 0;
	}

	size_t get_memory_usage() {
		return size * slot<T>::get_memory_usage();
	}

};

#endif