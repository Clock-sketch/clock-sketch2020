
#include<math.h>
#include <bitset>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <queue>
#include <time.h>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "../BOBHash32.h"
#include "bitmap_clock.h"
using namespace std;
const int MAXNUM = 3 * 1e7 + 10;
int flow[MAXNUM + 10];

void load_data1_CAIDA() {
	BOBHash32 hash_id;
	hash_id.initialize(rand() % MAX_PRIME32);

	ifstream input("/path/to/file", ios::in | ios::binary);
	char buf[2000] = { 0 };
	for (int i = 1; i <= MAXNUM; i++) {
		if (!input.read(buf, 21)) {
			printf("ERROR   %d\n", i);
			break;
		}
		flow[i] = hash_id.run(buf , 13);
	
	}
}



bitmap  bm;

BOBHash32 hash_id;
unordered_map<int, int> inmap;

double ans[MAXNUM + window + 10];

void test()
{
	for (int counter = 2; counter <= 8; counter *= 2) {
		for (int win = (1 << 10); win <= (1 << 16); win <<= 2) {
			for (int mem = (1 << 13); mem <= (1 <<19); mem <<= 1)
			{
				double sum = 0;
				int repmax = 1;
				int cnt = 0;
				for (int i = 0; i < repmax; i++) {
					inmap.clear();
					bm.init(win, mem, counter);
					double ans = 0;
					for (int time = 1; time <= win * 3; time++) {
						bm.insert(flow[time]);
						bm.update(time);
					}
					for (int k =win+1; k <= win * 3; k++) {
						inmap[flow[k]]++;
						if (inmap[flow[k]] == 1)
							ans++;
						inmap[flow[k - win]]--;
						if (inmap[flow[k - win]] == 0)
							ans--;
					}
					for (int time = win * 3 + 1; time <= win * 45; time++) {
						bm.insert(flow[time]);
						bm.update(time);
						inmap[flow[time]]++;
						if (inmap[flow[time]] == 1)
							ans++;
						inmap[flow[time - win]]--;
						if (inmap[flow[time - win]] == 0)
							ans--;
						if (time % (win*2) == 0) {
							double ss = 0;
							double bmcardality = bm.query();
							ss = bmcardality / ans;
							printf("query time %d\t%.6lf\tbmcardinality:%lf\trealcardality:%lf\n", time, fabs(ss - 1), bmcardality, ans);
							sum += fabs(ss - 1.0);
							cnt++;
						}
					}
				}
				printf(" counter %d \t windowsize:%d \t memory:%d Byte \t ave error:%.6lf\n", bm.countersize,win,  bm.memory / 8, sum / cnt);
			}
		}
	}
}
int main()
{  
    srand(clock());
	load_data1_CAIDA();
	test();
	return 0;
}
