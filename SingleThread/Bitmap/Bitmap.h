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
public:
    Bitmap(int _window, int _memory, int _hash_num, int _countersize=2);
    ~Bitmap();
    void insert(int x);
    void updateClock(int insertTimesPerUpdate=1);
    double query();
private:
    void updateRange(int beg, int end, int val);
};

void testBitmap_th(TRACE &trace, const int window_sz, const int memory, int clocksize, int hashnum=1);
void testBitmap_re(TRACE &trace, const int window_sz, const int memory, int clocksize, int hashnum=1);

#endif //_BITMAP_H_