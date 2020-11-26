#ifndef _BLOOM_FILTER_H_
#define _BLOOM_FILTER_H_

#include "../Main/BOBHash32.h"
#include "../Main/utils.h"

class BloomFilter
{
    int window_sz;
    int width;
    int hash_num;

    Clock_t *clocks;

    int lastUpdateIdx;
    int clocksize;
    int updateLen;
    BOBHash32 *hash_func;
public:
    BloomFilter(int _window, int _memory, int _hash_num, int _countersize=2);
    ~BloomFilter();
    void insert(int x);
    void updateClock(int insertTimesPerUpdate=1);
    bool query(int x);
private:
    void updateRange(int beg, int end, int val);
};

void testBloomFilter_fpr(TRACE &trace, const int window_sz, const int memory, int hashnum, int clocksize,
    int start_time, int end_time, int insertTimesPerQuery);
void testBloomFilter_th(TRACE &trace, const int window_sz, const int memory, int hashnum, int clocksize);

#endif //_BLOOM_FILTER_H_