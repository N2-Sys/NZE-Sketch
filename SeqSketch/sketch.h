#ifndef _SKETCH_H
#define _SKETCH_H

#include "BloomFilter.h"
#include "HashTable.h"
#include "CMsketch.h"

template<typename T1, typename T2, uint32_t slot_num, uint32_t depth, uint32_t width, uint32_t bf_width, int bf_hash>
class NZEsketch {
public:
	//data plane
	hashTable<T1, slot_num> hTable;
	CMsketch<T2, depth, width> cm;
	BloomFilter<bf_width, bf_hash> bf;

	//control plane
	std::vector<trace_t> flowKeys;
	std::map<trace_t, T1> evictResult;
	std::map<trace_t, T2> cmResult;

	NZEsketch() {
		flowKeys.clear();
		evictResult.clear();
		cmResult.clear();
	}

	int insert(Key_t key, uint32_t val = 1) {
		slot<T1> tempSlot;
		int evictOrNot = hTable.insert(tempSlot, key, val);

		if (evictOrNot == MISS_EVICT) {
			auto iter = evictResult.find(tempSlot.key);
			if (iter == evictResult.end()) {
				evictResult.emplace(tempSlot.key, tempSlot.val);
			}
			else {
				evictResult[tempSlot.key] += tempSlot.val;
			}
		}
		else if (evictOrNot == MISS_INSERT) {
			cm.insert(key, tempSlot.val);
			if (!bf.getbit(key)) {
				bf.setbit(key);
				flowKeys.push_back(tempSlot.key);
			}
		}
		return 0;
	}

	void solve_equations(std::map<trace_t, T2> & mp) {
		if (!mp.empty())
			return;

		uint32_t w = cm.width;
		int M = depth * w, N;

		//Unique flow keys
		std::sort(flowKeys.begin(), flowKeys.end());
		flowKeys.erase(std::unique(flowKeys.begin(), flowKeys.end()), flowKeys.end());
		N = flowKeys.size();

		Eigen::VectorXd X(N), b(M);
		Eigen::SparseMatrix<double> A(M, N);
		std::vector<Eigen::Triplet<double>> tripletlist;

		for (int i = 0, j; i < depth; i++) {
			j = 0;
			for (std::vector<trace_t>::iterator it = flowKeys.begin(); it != flowKeys.end(); ++it, ++j) {
				int idx = i * w + cm.hash((Key_t)it->str, i);
				//按Triplet方式填充，速度快
				tripletlist.push_back(Eigen::Triplet<double>(idx, j, 1));
			}
			for (j = 0; j < w; ++j) {
				b(i * w + j) = cm.matrix[i][j];
			}
		}
		
		A.setFromTriplets(tripletlist.begin(), tripletlist.end());

		 // 压缩优化矩阵
		A.makeCompressed();
		Eigen::LeastSquaresConjugateGradient<Eigen::SparseMatrix<double>> Solver_sparse;
		Solver_sparse.compute(A);
		X = Solver_sparse.solve(b);

		for (int i = 0; i < N; ++i) {
			auto iter = mp.find(flowKeys[i]);
			assert(iter == mp.end());
			int ans = (int)(X(i) + 0.5);
			mp.emplace(flowKeys[i], (ans <= 0)?1:ans);		//???可以这样吗
		}

		// for (auto iter = mp.begin(); iter != mp.end(); iter++) {
		// 	if (iter->second)
		// 		std::cout << iter->second << std::endl;
		// }
	}

	uint32_t query(Key_t key) {
		trace_t tempKey = trace_t(key);

		solve_equations(cmResult);

		T1 tableAns = hTable.query(key);

		T1 evictAns = 0;
		typename std::map<trace_t, T1>::iterator iterOfEvict = evictResult.find(tempKey);
		if (iterOfEvict != evictResult.end())
			evictAns = iterOfEvict->second;

		T2 cmAns = 0;
		typename std::map<trace_t, T2>::iterator iterOfCM = cmResult.find(tempKey);
		if (iterOfCM != cmResult.end())
			cmAns = iterOfCM->second;

		// std::cout << tableAns << " " << evictAns << " " << cmAns << std::endl;

		return (uint32_t)tableAns + (uint32_t)evictAns + (uint32_t)cmAns;
	}

	size_t get_memory_usage() {
		size_t htSize = hTable.get_memory_usage();
		size_t bfSize = bf.get_memory_usage();
		size_t cmSize = cm.get_memory_usage();
		std::cout << "----- Memory Usage -----" << std::endl;
		std::cout << "Hash Table Size(Byte):" << htSize << "(" << (double)htSize / 1024 / 1024 << "MB)" << std::endl;
		std::cout << "Bloom Filter Size(Byte):" << bfSize << "(" << (double)bfSize / 1024 / 1024 << "MB)" << std::endl;
		std::cout << "CM Sketch Size(Byte):" << cmSize << "(" << (double)cmSize / 1024 / 1024 << "MB)" << std::endl;
		std::cout << "Total Memory(MB):" << ((double)htSize + (double)bfSize + (double)cmSize) / 1024 / 1024 << std::endl;
		std::cout << "------------------------" << std::endl;
		return htSize + bfSize + cmSize;
	}
};

#endif