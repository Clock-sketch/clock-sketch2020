
#include "parameter.h"
#include "cm_clock.cpp"

TRACE traces[END_FILE_NO - START_FILE_NO + 1];

void ReadTraces() {
	for (int datafileCnt = START_FILE_NO; datafileCnt <= END_FILE_NO; datafileCnt++) {
		char datafileName[100];
		sprintf(datafileName, "/path/to/file", datafileCnt);

		FILE* fin = fopen(datafileName, "rb");
		cout << datafileName << endl;

		FIVE_TUPLE temp_five_tuple;
		traces[datafileCnt].clear();

		while (fread(&temp_five_tuple, 1, ITEM_LENGTH, fin) == ITEM_LENGTH) {
			traces[datafileCnt].push_back(temp_five_tuple);
		}

		fclose(fin);
		printf("Successfully read in %s,%ld packets\n", datafileName, traces[datafileCnt].size());
	}
	cout << endl;
}

int findwindow(const TRACE& trace ,int t, FREQ_RECORD& acc,int win ) {
	
		for (int i = 0; i < win; i++) {
			string key1((const char*)(trace[t-i].key + 8), KEY_LENGTH);
			if (acc.count(key1) <= 0) {
				
				acc[key1] .cnt=1;
				acc[key1].time = t - i;
				for (int j = t - i; j >= 0&&j+win>acc[key1].time; j--) {
					string key2((const char*)(trace[j].key + 8), KEY_LENGTH);
					if (key2 == key1) {
						acc[key1].cnt++;
						acc[key1].time = j;
					}
				}
			}	
		}
	return 0;
}

int test_trace(const TRACE& trace, const int trace_id, const int window_sz, const int memory, int clocksize) {

	int time_cnt = 0;
	int window_num = 0;

	int width = memory / (8 * (16 + clocksize));
	cm_sketch cmsket(window_sz, 8, width, clocksize);

	while (time_cnt <= window_sz*5) {

		string key1((const char*)(trace[time_cnt].key+8 ), KEY_LENGTH);
		cmsket.insert(time_cnt, key1.c_str(), 1, clocksize);
		cmsket.updateclock(time_cnt, clocksize);
		time_cnt++;
	}
	double  are = 0;
	double  per=0;

	for (int time_cnt = 5*window_sz + 1; time_cnt < traces->size() && window_num <= 20; time_cnt++) {

		string key1((const char*)(trace[time_cnt].key+8), KEY_LENGTH);
		cmsket.insert(time_cnt, key1.c_str(), 1, clocksize);
		cmsket.updateclock(time_cnt, clocksize);
		double per1=0;
		if (time_cnt % (window_sz) == 0) {
			int op=0;
			FREQ_RECORD Real_Freq;
			findwindow(trace, time_cnt, Real_Freq, window_sz);
			double tep = cmsket.calc_ARE(time_cnt, Real_Freq,op);
			are += tep;
			per+=double(op)/(double)Real_Freq.size();

			printf("countsize:%d \t windowsz:%d \t mem:%d \t are:%lf \t it : %lf\t per:%lf\n",
				clocksize,
				window_sz,
				memory >> 3,
				tep,
				cmsket.getit(),
				double(op)/(double)Real_Freq.size());
            fflush(stdout);
			window_num++;
		}
	}
	printf("countsize:%d \t windowsz:%d \t mem:%d \t are:%lf \t percent: %lf\n",
		clocksize,
		window_sz,
		memory >> 3,
		are / (window_num),
		per / window_num);
    fflush(stdout);
	return 0;
}

int main() {
	ReadTraces();
	printf("window, trace id, window size, flow amount, memory, ARE\n");
    srand(clock());
	for (int i = START_FILE_NO; i <= END_FILE_NO; i++) {
		for (int count = 2; count <= 5; count++) {
			for (int win = (1 << 10); win <= (1 << 16); win <<= 2) {
				for (int mem = (1 << 19); mem <= (1 << 23); mem <<= 1) {
					test_trace(traces[i], i, win, mem, count);
				}
			}
		}
	}

	return 0;
}





