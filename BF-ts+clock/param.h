#include<vector>
#include<iostream>
#include<unordered_map>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<deque>
#include"BOBHash32.h"
#define MYLIMIT lim//抛弃长度过短的包
int mylimit[4] = { 50,150,400,1000 };
#define STREAMSIZE (1<<21)//数据流元素总个数
#define TAKE_TIMESTAMP_INTO_ACCOUNT 0
#define random(a,b) (rand()%(b-a)+a)//产生[a,b)之间的随机整数
#define TIME_STAMP uint64_t
#define ID uint32_t
#define END_FILE_NO 0
#define START_FILE_NO 0
#define KEY_LENGTH 4
#define ITEM_LENGTH 4
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a) ((a)>0?(a):(-(a)))
struct FIVE_TUPLE{
	char key[ITEM_LENGTH];
};
typedef vector<FIVE_TUPLE> TRACE;
typedef unordered_map<string, int> FREQ_RECORD;
