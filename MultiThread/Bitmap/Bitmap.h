#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "../Main/BOBHash32.h"
#include "../Main/utils.h"

class Bitmap
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
    Bitmap(int _window, int _memory, int _hash_num, int _countersize=2, int _insertTimesPerUpdate=1);
    ~Bitmap();
    void insert(int x);
    void updateClock();
    double query();
    void Insert(int j, int flow[]);
private:
    void updateRange(int beg, int end, int val);
    static void updateThread(Bitmap *bm);
};

void testBitmap_th(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate, int hashnum=1);
void testBitmap_re(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate, int hashnum=1);

#endif //_BITMAP_H_