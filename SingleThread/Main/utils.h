#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <string.h>
using std::vector;
using std::string;
using std::unordered_map;

typedef unsigned short Counter_t;
typedef unsigned short Clock_t;

#define ITEM_LENGTH 16
#define KEY_LENGTH 4

struct FIVE_TUPLE
{
    char key[ITEM_LENGTH];
};
typedef vector<FIVE_TUPLE> TRACE;

struct se
{   
    int cnt;
    int time;
};  
typedef unordered_map<string, se> FREQ_RECORD;

#define START_FILE_NO 0
#define END_FILE_NO 0
void ReadTraces(TRACE* traces);


#endif //_UTILS_H_