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

    thread updator;
    int insertTimesPerUpdate;
    std::atomic<int> updating;
    std::atomic<int> blocking;
    std::atomic<int> whileing;
public:
    BloomFilter(int _window, int _memory, int _hash_num, int _countersize=2, int _insertTimesPerUpdate=1);
    ~BloomFilter();
    void insert(int x);
    void updateClock();
    bool query(int x);
    void Insert(int j, int flow[]);
private:
    void updateRange(int beg, int end, int val);
    static void updateThread(BloomFilter *bf);
};

void testBloomFilter_fpr(TRACE &trace, int window_sz, const int memory, int hashnum, int clocksize,
    int start_time, int end_time, int insertTimesPerQuery, int insertTimesPerUpdate);
void testBloomFilter_th(TRACE &trace, int window_sz, const int memory, int hashnum, int clocksize,
    int insertTimesPerUpdate);

#endif //_BLOOM_FILTER_H_