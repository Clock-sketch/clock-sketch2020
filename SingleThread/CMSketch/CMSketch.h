#ifndef _CM_SKETCH_CLOCK_H_
#define _CM_SKETCH_CLOCK_H_

#include "../Main/BOBHash32.h"
#include "../Main/utils.h"

class CMSketch
{
    int window_sz;
    int d;
    int width;
    int W;      // W = d * width

    Counter_t *counters;
    Clock_t *clocks;

    int lastUpdateIdx;
    int clockSize;
    BOBHash32 *hash_func;
public:
    CMSketch(int window_sz_, int d_, int width_, int clockSize_);
    ~CMSketch();
    int getWindowSz();
    void insert(int time_cnt, const char *key, Counter_t f, int clockSz);
    void updateClock(int insertTimesPerUpdate=1);
    int query(const char *key);
    double calc_ARE(int time_cnt, const FREQ_RECORD &Real_Freq);
private:
    void updateRange(int beg, int end, int val);
};

void testCMSketch_are(TRACE &trace, const int window_sz, const int memory, int clocksize);
void testCMSketch_th(TRACE &trace, const int window_sz, const int memory, int clocksize);

#endif //_CM_SKETCH_CLOCK_H_