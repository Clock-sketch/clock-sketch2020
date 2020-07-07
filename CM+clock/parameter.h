#include<vector>
#include<iostream>
#include<unordered_map>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<deque>
#include<ctime>

#define END_FILE_NO 0
#define START_FILE_NO 0
#define KEY_LENGTH 8
#define ITEM_LENGTH 16
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a) ((a)>0?(a):(-(a)))

using namespace std;
struct se {
	int cnt;
	int lasttime;
	int time;
};

struct FIVE_TUPLE {
	char key[ITEM_LENGTH];
};
typedef vector<FIVE_TUPLE> TRACE;

typedef unordered_map<string, se> FREQ_RECORD;