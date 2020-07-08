#include "param.h"
#include "mybloomfilter.h"
#pragma warning(disable:4996)
TRACE traces[END_FILE_NO - START_FILE_NO + 1];
void ReadInTraces() {
	for (int datafileCnt = START_FILE_NO; datafileCnt <= END_FILE_NO; datafileCnt++) {
		char datafileName[100];
		sprintf(datafileName, "./net.dat", datafileCnt);

		FILE *fin = fopen(datafileName, "rb");
		cout << datafileName << endl;

		FIVE_TUPLE tmp_five_tuple;
		traces[datafileCnt].clear();
		int datacnt = 0;
		while (fread(&tmp_five_tuple, 1, ITEM_LENGTH, fin) == ITEM_LENGTH && datacnt < STREAMSIZE) {
			traces[datafileCnt].push_back(tmp_five_tuple);
			datacnt++;
		}
		fclose(fin);
		printf("Successfully read in %s, %ld packets\n", datafileName, traces[datafileCnt].size());
	}
	printf("\n");
}

int test_trace(TRACE& trace, BF& bloomfilter, int start_time, int end_time, int freq) 
{
	
	FREQ_RECORD real_freq;
	TIME_STAMP* time_stamp=new TIME_STAMP[end_time];
	ID* id = new ID[end_time];
	for (int cnt = 0; cnt < end_time; cnt++) {
		time_stamp[cnt] = *(TIME_STAMP*)(&trace[cnt]);
		id[cnt] = *((ID*)(&trace[cnt])+2)+ *((ID*)(&trace[cnt]) + 3);
		//cout << time_stamp[cnt] << ' ' << id[cnt] << endl;
	}
	clock_t starttime = clock();
	bloomfilter.calc_throughput(id, start_time, end_time, freq);
	//printf("%d,%d,%d,%.8f", start_time,bloomfilter.window,bloomfilter.memory, bloomfilter.calc_FPR(id,start_time,end_time,freq));
	clock_t endtime = clock();

	delete[]time_stamp;
	delete[]id;
	return 0;
}
int main() {
	ReadInTraces();
	int freq = 1 << 2;
	printf("starttime,windowsize,memory,ARE,RE\n");
	for (int i = START_FILE_NO; i <= END_FILE_NO; i++) {
		int b = 8;
		for(int wsize=1<<10;wsize<=1<<16;wsize*=4)
			for (int mem = 2 * 32 * 1024; mem <= 2 * 256 * 1024; mem *= 2) 
				 {
					BF bf = BF(wsize, mem, 8);//windowsize = wsize, memory=mem,hashnum = 8,bit_per_cell=b
					test_trace(traces[i], bf, 6*wsize,1<<21, freq);//starttime=0,endtime=1<<21,freq=4	

				}
	}
	system("pause");
	return 0;
}

