//BF(naive)

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <bitset>
#include <time.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include<random>
#include "./BOBHash32.h"
using namespace std;

class BF
{
public:
	static const int
		MAX_CELL_NUM = 1e7,
		MAX_HASH_NUM = 50;
	int
		window, //window size
		memory,
		hash_num, //number of hash function
		max_cell_num,
		group_size;
	double bias = 0;//由于每次计算出来的刷新的cell数可能不是整数，其小数部分在此累积，以减小误差
	BOBHash32* hash;
	BOBHash32 hash_time_offset;
	TIME_STAMP* last_time;
	TIME_STAMP* first_time;
	//uint8_t* bf;
	int current_time;
	int current_pointer = 0;//更新时指针
	int pointer_speed;//指针移动的速度
	BF(int _window, int _memory,
		int _hash_num)
	{
		window = _window,
			memory = _memory,
			hash_num = _hash_num,
			max_cell_num = memory/(8+8),//要算上记录上次抵达时间的last_time数组的大小
		hash = new BOBHash32[hash_num];
		last_time = new TIME_STAMP[max_cell_num];
		first_time = new TIME_STAMP[max_cell_num];
		current_time = 0;
		//pointer_speed = ((1<<bit_per_cell)-2)*max_cell_num/window;
		for (int i = 0; i < hash_num; i++)
		{
			hash[i].initialize(rand() % MAX_PRIME32);
		}
		hash_time_offset.initialize(rand() % MAX_PRIME32);
		for (int i = 0; i < max_cell_num; i++)
		{
			last_time[i] = 0;
			first_time[i] = 0;
		}
	}
	~BF() {
		delete[]hash;
		delete[]first_time;
		delete[]last_time;

	}
	
	void refresh(long long times = 1) {//times表示时间戳的改变量
		times = times;
		return;
	}
	int query(int x)//查询x持续出现的时间
	{
		int firsttime = 0;
		for (int i = 0; i < hash_num; i++)
		{
			int pos = hash[i].run((char *)&x, sizeof(int)) % max_cell_num;
			if (first_time[pos] == 0||last_time[pos]==0)
				return 0;
			if (first_time[pos] > firsttime)
				firsttime = first_time[pos];
		}
		return current_time- firsttime +1;
	}
	void insert(int x)
	{
		current_time++;
		for (int i = 0; i < hash_num; i++)
		{
			int pos = hash[i].run((char *)&x, sizeof(int)) % max_cell_num;
			if (last_time[pos] == 0||current_time-last_time[pos]>window)
				first_time[pos] = current_time;
			last_time[pos] = current_time;
		}
		refresh(1);
	}
	
	double calc_FPR(ID* stream, int start_time, int end_time, int freq)//三个参数分别表示：查询开始的时间，查询结束的时间，每几个数查询一次
	{
		int lim;
		switch (window)

		{
		case 1 << 10:MYLIMIT = mylimit[0]; break;
		case 1 << 12:MYLIMIT = mylimit[1]; break;
		case 1 << 14:MYLIMIT = mylimit[2]; break;
		case 1 << 16:MYLIMIT = mylimit[3]; break;
		default:
			cout << "err" << endl;
			break;
		}
		double total_err = 0;
		double are = 0;
		int query_time = 0;
		for (int i = 0; i < end_time; i++) {//i代表当前时间
		//先插入数据,后查询
		
			//插入数据
			insert(stream[i]);
			//查询部分
			if (i >= start_time && (i - start_time) % freq == 0) {
				
				int q = stream[i];//查询q的长度（注：这时stream[i]刚刚被插入）
				int trueans = 0;//真实答案
				int j = i - 1;
				int last = i;
				int bagcnt = 1;//统计数据流出现的次数，若小于3则舍弃
				for (; j >= 0; j--) {
					if (stream[j] == q)
					{
						last = j;
						if (j >= i - window)
							bagcnt++;
					}
					if (last - j > window)
						break;
				}
				trueans = i - last + 1;
				if (trueans >= MYLIMIT) {
					int ans = query(q);
					query_time++;
					if (ans != trueans)
						total_err++;
					are += (double)ABS(trueans - ans) / trueans;
				}
				
			}
			
		}
		printf("%d,%d,%d,%.8f,%.8f\n", start_time, window, memory, (double)are/query_time,(double)total_err / query_time);
		//printf("total,length,number,both,either\n%d,%d,%d,%d,%d",query_time,length,number,both,either);
		return (double)total_err / query_time;//FPR
	}
	double calc_throughput(ID* stream, int start_time, int end_time, int freq)//三个参数分别表示：查询开始的时间，查询结束的时间，每几个数查询一次
	{
		clock_t starttime = clock();
		for (int i = 0; i < end_time; i++) {//i代表当前时间			
			insert(stream[i]);
		}
		clock_t endtime = clock();
		double thr1 = (double)end_time / (endtime - starttime);
		starttime = clock();
		for (int i = 0; i < end_time; i++) {//i代表当前时间			
			query(random(1, INT_MAX));
		}
		endtime = clock();
		double thr2 = (double)end_time / (endtime - starttime);
		printf("%d,%d,%f,%f\n", window, memory,  thr1, thr2);
		return 0;
	}
};