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

    thread updator;
    int insertTimesPerUpdate;
    std::atomic<int> updating;
    std::atomic<int> blocking;
    std::atomic<int> whileing;
public:
    CMSketch(int window_sz_, int d_, int width_, int clockSize_, int _insertTimesPerUpdate=1);
    ~CMSketch();
    int getWindowSz();
    void insert(int time_cnt, const char *key, Counter_t f, int clockSz);
    void updateClock();
    int query(const char *key);
    double calc_ARE(int time_cnt, const FREQ_RECORD &Real_Freq);
    void Insert(int j, int flow[]);
private:
    void updateRange(int beg, int end, int val);
    static void updateThread(CMSketch *cm);
};

void testCMSketch_are(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate);
void testCMSketch_th(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate);

#endif //_CM_SKETCH_CLOCK_H_