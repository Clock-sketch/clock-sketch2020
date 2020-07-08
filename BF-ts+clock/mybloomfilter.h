

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
	double bias = 0;
	BOBHash32* hash;
	BOBHash32 hash_time_offset;
	TIME_STAMP* last_time;
	uint8_t* bf;
	int current_time;
	int current_pointer = 0;
	int pointer_speed;
	BF(int _window, int _memory,
		int _hash_num,int _bit_per_cell)
	{
		window = _window,
			memory = _memory,
			hash_num = _hash_num,
			max_cell_num = 8 * memory/(_bit_per_cell+64),
			bit_per_cell=_bit_per_cell;
		hash = new BOBHash32[hash_num];
		last_time = new TIME_STAMP[max_cell_num];
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
			last_time[i] = 0;
			bf[i] = 0;
		}
	}
	~BF() {
		delete[]hash;
		delete[]bf;
		delete[]last_time;

	}
	
	void refresh(long long times = 1) {
		long long refreshedbits = (long long)(pointer_speed * times);
		bias += pointer_speed * times - refreshedbits;
		if (bias >= 1) {
			refreshedbits++;
			bias--;
		}
		if (refreshedbits >= ((1 << bit_per_cell) - 1) * max_cell_num)
		{
			for (int i = 0; i < max_cell_num; i++)
			{
				bf[current_pointer]  = 0;
			}
			current_pointer = (current_pointer + refreshedbits) % max_cell_num;
		}
		else {
			for (int i = 0; i < (refreshedbits); i++) {			
				if(bf[current_pointer]!=0)
					bf[current_pointer]--;				
				current_pointer = (current_pointer + 1) % max_cell_num;
			}
		}
	}
	int query(int x)
	{
		int latesttime = 0;
		for (int i = 0; i < hash_num; i++)
		{
			int pos = hash[i].run((char *)&x, sizeof(int)) % max_cell_num;
			if (bf[pos] == 0)
				return 0;
			if (last_time[pos] > latesttime)
				latesttime = last_time[pos];
		}
		return current_time- latesttime +1;
	}
	void insert(int x)
	{
		current_time++;
		for (int i = 0; i < hash_num; i++)
		{
			int pos = hash[i].run((char *)&x, sizeof(int)) % max_cell_num;
			if (bf[pos] == 0)
				last_time[pos] = current_time;
			bf[pos] = (1<<bit_per_cell)-1;
		}
		refresh(1);
	}
	
	double calc_FPR(ID* stream, int start_time, int end_time, int freq)
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
		int length = 0;
		int number = 0;
		int either = 0;
		int both = 0;
		for (int i = 0; i < end_time; i++) {
	
		

			insert(stream[i]);

			if (i >= start_time && (i - start_time) % freq == 0) {
				
				int q = stream[i];
				int trueans = 0;
				int j = i - 1;
				int last = i;
				int bagcnt = 1;
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
					length++;
					query_time++;
					int ans = query(q);
					
					if (ans != trueans)
						total_err++;
					are += (double)ABS(trueans - ans) / trueans;
				}
				
			}
			
		}
		printf("%d,%d,%d,%.8f\n", start_time, window, memory,(double)total_err / query_time);
		//printf("total,length,number,both,either\n%d,%d,%d,%d,%d",query_time,length,number,both,either);
		return (double)total_err / query_time;//FPR
	}
};