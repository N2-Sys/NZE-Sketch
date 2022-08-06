#ifndef _COMMON_H
#define _COMMON_H

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>

#include "AwareHash.h"
#include "Eigen/Dense"
#include "Eigen/IterativeLinearSolvers"
#include "Eigen/SparseCore"
// #include "parameters.h"

//Key length
#define KEY_T_SIZE 13

typedef const char* Key_t;

typedef struct trace_t {
	char str[KEY_T_SIZE];
	bool operator < (const trace_t& a) const {
		bool flag = false;
		for (int i = 0; i < KEY_T_SIZE; ++i) {
			if (this->str[i] == a.str[i])
				continue;
			else if (this->str[i] < a.str[i])
				flag = true;
			break;
		}
		return flag;
	}
	bool operator == (const trace_t& a) const {
		for (int i = 0; i < KEY_T_SIZE; ++i) {
			if (this->str[i] == a.str[i])
				continue;
			else
				return false;
		}
		return true;
	}
	bool isEmpty() {
		for (int i = 0; i < KEY_T_SIZE; ++i) {
			if (str[i] == 0)
				continue;
			else
				return false;
		}
		return true;
	}
	trace_t() {
		memset(str, 0, KEY_T_SIZE);
	}
	trace_t(char * s) {
		memcpy(str, s, KEY_T_SIZE);
	}
	trace_t(const char * s) {
		memcpy(str, s, KEY_T_SIZE);
	}
} trace_t;

bool isPrime(uint32_t num) {
	uint32_t border = (uint32_t)ceil(sqrt((double)num));
	for (uint32_t i = 2; i <= border; ++i) {
		if ((num % i) == 0)
			return false;
	}
	return true;
}

uint32_t calNextPrime(uint32_t num) {
	while (!isPrime(num)) {
		num++;
	}
	return num;
}

#endif


