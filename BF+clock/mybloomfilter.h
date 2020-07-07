//BF(进阶版,指针扫描)

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
		group_size,
		bit_per_cell;//number of bits per cell
	BOBHash32* hash;
	BOBHash32 hash_time_offset;
	//bool time_stamp[MAX_CELL_NUM];
	uint8_t* bf;
	int current_time;
	int current_pointer = 0;//更新时指针
	int pointer_speed;//指针移动的速度
	BF(int _window, int _memory,
		int _hash_num,int _bit_per_cell)
	{
		window = _window,
			memory = _memory,
			hash_num = _hash_num,
			max_cell_num = 8 * memory/_bit_per_cell,
			bit_per_cell=_bit_per_cell;
		hash = new BOBHash32[hash_num];
		
		bf = new uint8_t[max_cell_num];
		current_time = 0;
		pointer_speed = ((1<<bit_per_cell)-2)*max_cell_num/window;
		for (int i = 0; i < hash_num; i++)
		{
			hash[i].initialize(rand() % MAX_PRIME32);
		}
		hash_time_offset.initialize(rand() % MAX_PRIME32);
		for (int i = 0; i < max_cell_num; i++)
		{
			bf[i] = 0;
		}
	}
	~BF() {
		delete[]hash;
		delete[]bf;

	}
	//bool get_timestamp(int tx, int pos)
	//{
	//    int gid = pos / group_size;
	//    //int t_add = hash_time_offset.run((char *)&gid, sizeof(int)) & ((1 << 29) - 1);
	//    int t_add = relax_window_size * gid / (max_cell_num / group_size);
	//    return int((tx + t_add) / (alpha * window)) & 1;
	//}
	//void check_timestamp(int pos)
	//{
	//    int gid = pos / group_size;

	//    if (get_timestamp(current_time, pos) == time_stamp[gid])
	//        return;
	//    time_stamp[gid] = get_timestamp(current_time, pos);

	//    int st = pos / group_size * group_size;
	//    for (int i = st; i < st + group_size && i < max_cell_num; i++)
	//        bf[i] = 0;
	//}
	void refresh() {
		for (int i = 0; i < pointer_speed; i++) {
			if (bf[current_pointer] != 0)
				bf[current_pointer]--;
			current_pointer = (current_pointer + 1) % max_cell_num;
		}
	}
	void insert(int x)
	{
		current_time++;
		for (int i = 0; i < hash_num; i++)
		{
			int pos = hash[i].run((char *)&x, sizeof(int)) % max_cell_num;
			//check_timestamp(pos);
			bf[pos] = (1<<bit_per_cell)-1;
		}
		refresh();
	}
	bool query(int x)
	{
		for (int i = 0; i < hash_num; i++)
		{
			int pos = hash[i].run((char *)&x, sizeof(int)) % max_cell_num;
			if (bf[pos] == 0)
				return 0;
		}
		return 1;
	}
	double calc_FPR(ID* stream, int start_time, int end_time, int freq)//三个参数分别表示：查询开始的时间，查询结束的时间，每几个数查询一次
	{
		int TP = 0, FP = 0, FN = 0, TN = 0;
		for (int i = 0; i < end_time; i++) {//i代表当前时间
		//先查询，后插入数据
		//查询部分

			if (i >= start_time && (i - start_time) % freq == 0) {
				int q = random(0, INT32_MAX);//查询q是否在bloom中
				bool ans = query(q);
				bool trueans = 0;//真实答案
				for (int j = i - 1; j >= i - 1.5*window && j >= 0; j--)
					if (stream[j] == q)
					{
						trueans = 1;
						break;
					}
				if (ans)
					if (trueans)//真阳性
						TP++;
					else//假阳性
						FP++;
				else
					if (trueans)//假阴性
						FN++;
					else//真阴性
						TN++;

			}
			//插入数据
			insert(stream[i]);
		}
		double FPR = (double)FP / (FP + TN);
		printf("%d,%d,%d,%f\n", window, memory, bit_per_cell, FPR);
		return FPR;
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
			query(random(1,INT_MAX));
		}
		endtime = clock();
		double thr2 = (double)end_time / (endtime - starttime);
		printf("%d,%d,%d,%f,%f\n",window,memory, bit_per_cell, thr1, thr2);
		return 0;
	}
};