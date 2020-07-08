#include "param.h"
#include "mybloomfilter.h"
#pragma warning(disable:4996)
TRACE traces[END_FILE_NO - START_FILE_NO + 1];
void ReadInTraces() {
	for (int datafileCnt = START_FILE_NO; datafileCnt <= END_FILE_NO; datafileCnt++) {
		char datafileName[100];
		sprintf(datafileName, "./formatted00.dat", datafileCnt);

		FILE *fin = fopen(datafileName, "rb");
		cout << datafileName << endl;

		FIVE_TUPLE tmp_five_tuple;
		traces[datafileCnt].clear();
		int datacnt = 0;
		while (fread(&tmp_five_tuple, 1, ITEM_LENGTH, fin) == ITEM_LENGTH&&datacnt<STREAMSIZE) {
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
		id[cnt] = *((ID*)(&trace[cnt]));
		//cout << id[cnt] << endl;
	}
	clock_t starttime = clock();
	bloomfilter.calc_FPR(id,start_time,end_time,freq);
	clock_t endtime = clock();
	//bloomfilter.calc_throughput(id, start_time, end_time, freq);
	delete[]time_stamp;
	delete[]id;
	return 0;
}
int main() {
	ReadInTraces();
	int freq = 1<<3;
	printf(" cell λ FPR \n");
	printf("window,memory,bit_per_cell,FPR\n");
	for (int i = START_FILE_NO; i <= END_FILE_NO; i++) {
		for(int win=1<<16;win<=1<<16;win*=2)
			for(int mem=8*1024;mem<=256*1024;mem*=2)
				for (int b = 2; b <=2; b++) 
					for(int start=0;start<=0;start+=win)
				{
						int hashnum = 1+(0.6931*mem * 8) / (win*b);
						
					
					BF bf = BF(win, mem, hashnum, b);//windowsize = 1<<16, memory=128KB,hashnum = 8,bit_per_cell=b
					test_trace(traces[i], bf, start, 1<<21, freq);//starttime=0,endtime=1<<21,freq=4		
				}
	}
	system("pause");
	return 0;
}

