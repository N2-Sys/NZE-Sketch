#ifndef _CMSKETCH_H
#define _CMSKETCH_H

#include "common.h"
#include "parameters.h"

template<typename T, uint32_t d, uint32_t w>
class CMsketch {
	public:
		uint32_t depth = d, width = calNextPrime(w);
		uint64_t h[d], s[d], n[d];
		std::vector<std::vector<T>> matrix;
		
		CMsketch() {
			matrix = std::vector< std::vector<T> >(depth, std::vector<T>(width, 0));
			int index = 0;
			for (int i = 0; i < depth; ++i) {
				h[i] = GenHashSeed(index++);
				s[i] = GenHashSeed(index++);
				n[i] = GenHashSeed(index++);
			}
		}

		uint32_t hash(Key_t key, int col) {
			return (uint32_t)(AwareHash((unsigned char *)key, KEY_T_SIZE, h[col], s[col], n[col]) % width);
		}

		int insert(Key_t key, T val = 1) {
			uint32_t pos;
			for (int i = 0; i < depth; ++i) {
				pos = hash(key, i);
				assert(matrix[i][pos] != std::numeric_limits<T>::max());
				matrix[i][pos] += val;
			}
			return 0;
		}
		
		T query(Key_t key) {
			uint32_t pos;
			T result = std::numeric_limits<T>::max();
			for (int i = 0; i < depth; ++i) {
				pos = hash(key, i);
				result = std::min(result, matrix[i][pos]);
			}
			return result;
		}

		size_t get_memory_usage() {
			return depth * width * sizeof(T);
		}
};

#endif
