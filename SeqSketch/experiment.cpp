#include "sketch.h"

using namespace std;

string path = "../data/test-8s.dat";
vector<trace_t> traces;
map<trace_t, uint32_t> ground_truth;
NZEsketch<uint32_t, uint32_t, SLOT_NUM, CM_DEPTH, CM_WIDTH, BLOOM_SIZE, BLOOM_HASH_NUM> nze;

int readTraces(const char *path) {
	FILE *inputData = fopen(path, "rb");

	assert(inputData != NULL);

	traces.clear();
	char *strData = new char[KEY_T_SIZE];

	printf("Reading in data\n");

	while (fread(strData, KEY_T_SIZE, 1, inputData) == 1) {
		traces.push_back(trace_t(strData));
	}
	fclose(inputData);
	
	int size = traces.size();

	printf("Successfully read in %d packets\n", size);

	return size;
}

int main(int argc, char *argv[]) {
	//read the traces

	if (argc > 1) {
		path = argv[1];
	}

	int size = readTraces(path.c_str());
	int break_number = 100000;
	double are = 0, perFlowARE = 0;
	int packetCnt = 0, staisfiedFlowCnt = 0;
	bool lastFlow = false;

	/********************* NZE sketch ***************************/

	//get the ground truth & insert
	for (int i = 0; i < size; ++i) {
		if (ground_truth.find(traces[i]) != ground_truth.end()) {
			ground_truth[traces[i]] += 1;
		}
		else if (!lastFlow) {
			ground_truth[traces[i]] = 1;
			if (ground_truth.size() == break_number)
				lastFlow = true;
		}
		else {
			continue;
		}

		nze.insert((Key_t)traces[i].str);
		packetCnt++;
	}
	cout << "Insert " << packetCnt << " packets and " << break_number << " flows" << endl;

	for (auto it = ground_truth.begin(); it != ground_truth.end(); it++) {
		uint32_t ans = nze.query((Key_t)it->first.str);
		if (ans != it->second) {
			perFlowARE = fabs((double)ans - (double)it->second) / (double)it->second;
			are += perFlowARE;
			if (perFlowARE <= 0.001) {
				staisfiedFlowCnt++;
			}
			// cout << ans << " " << it->second << endl;
		}
		else {
			staisfiedFlowCnt++;
		}
	}

	size_t nzeSize = nze.get_memory_usage();

	are /= ground_truth.size();
	cout << "ARE of NZE:" << are << endl;
	cout << "Satisfied flow proportion:" << (double)staisfiedFlowCnt / break_number * 100 << "%" << endl;

	return 0;
}




