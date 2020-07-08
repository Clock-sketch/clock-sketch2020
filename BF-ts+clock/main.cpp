#include "param.h"
#include "mybloomfilter.h"
#pragma warning(disable:4996)
TRACE traces[END_FILE_NO - START_FILE_NO + 1];
void ReadInTraces() {
	for (int datafileCnt = START_FILE_NO; datafileCnt <= END_FILE_NO; datafileCnt++) {
		char datafileName[100];
		sprintf(datafileName, "webdocs_form00.dat", datafileCnt);

		FILE *fin = fopen("webdocs_form00.dat", "rb");
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
	TIME_STAMP* time_stamp = new TIME_STAMP[end_time];
	ID* id = new ID[end_time];
	for (int cnt = 0; cnt < end_time; cnt++) {
		if (TAKE_TIMESTAMP_INTO_ACCOUNT)
			time_stamp[cnt] = *(TIME_STAMP*)(&trace[cnt]) / 4294967296;
		else
			time_stamp[cnt] = cnt;
		id[cnt] = *((ID*)(&trace[cnt]));
		/*if (cnt > 0 && cnt % (1<<16) == 0)
			cout << (time_stamp[cnt] - time_stamp[cnt - (1<<16)]) << endl;*/
	}
	clock_t starttime = clock();
	bloomfilter.calc_FPR(id, start_time, end_time, freq);
	
	clock_t endtime = clock();
	delete[]time_stamp;
	delete[]id;
	return 0;
}
int main() {
	ReadInTraces();
	int freq = 5;
	TIME_STAMP wsize;
	if (TAKE_TIMESTAMP_INTO_ACCOUNT)
		wsize = 125228;
	else
		wsize = 1 << 16;
	printf("item_batch window memory FPR\n");
	printf("starttime,windowsize,memory,FPR\n");
	for (int i = START_FILE_NO; i <= END_FILE_NO; i++) {
		for (int b = 8; b <= 8; b *= 2) {
			printf("bit per cell = %d\n", b);
			for (int wsize = 1 << 10; wsize <= 1 << 10; wsize *= 4)
				for (int mem = 64* 1024; mem <= 512 * 1024; mem *= 2)
					
				{
					BF bf = BF(wsize, mem, 8, b);//windowsize = wsize, memory=mem,hashnum = 8,bit_per_cell=b
					test_trace(traces[i], bf, 6*wsize, 1<<21, freq);//starttime=0,endtime=1<<21,freq=4	

				}
		}
	}
	system("pause");
	return 0;
}

