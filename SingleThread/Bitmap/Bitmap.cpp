#include <math.h>
#include <immintrin.h> // AVX
#include "Bitmap.h"

Bitmap::Bitmap(int _window, int _memory, int _hash_num, int _countersize)
    : window_sz(_window), hash_num(_hash_num), lastUpdateIdx(0), clocksize(_countersize)
{
    width = 8 * _memory / _countersize;
    updateLen = ((1 << _countersize) - 2) * width / _window;

    clocks = new Clock_t[width];
    memset(clocks, 0, sizeof(Clock_t) * width);

    hash_func = new BOBHash32[hash_num];
    for(int i = 0; i < hash_num; ++i)
        hash_func[i].initialize(100 + i);
}

Bitmap::~Bitmap()
{
    delete[] clocks;
    delete[] hash_func;
}

void Bitmap::insert(int x)
{
    for(int i = 0; i < hash_num; ++i)
    {
        int pos = hash_func[i].run((char*)&x, sizeof(int)) % width;
        clocks[pos] = (1 << clocksize) - 1;
    }
}

void Bitmap::updateClock(int insertTimesPerUpdate)
{
    int temp = updateLen * insertTimesPerUpdate;
    int subAll = temp / width;
    int len = temp % width;

    int beg = lastUpdateIdx, end = std::min(width, lastUpdateIdx + len);
    updateRange(beg, end, subAll + 1);
    if(end - beg < len)
    {
        end = len - (end - beg);
        beg = 0;
        updateRange(beg, end, subAll + 1);
    }

    if(end > lastUpdateIdx){
        updateRange(end, width, subAll);
        updateRange(0, lastUpdateIdx, subAll);
    }
    else
        updateRange(end, lastUpdateIdx, subAll);
    lastUpdateIdx = end;
}

double Bitmap::query()
{
    double u = 0;
    for(int i = 0; i < width; ++i)
        u += clocks[i] == 0 ? 1 : 0;
    return -width * log(u / width);
}

void Bitmap::updateRange(int beg, int end, int val)
{
    if(val <= 0)    return;
    
    __m256i _subVal = _mm256_set1_epi16(short(val));
    while(beg + 16 <= end)
    {
        __m256i clock = _mm256_loadu_si256((__m256i*)&clocks[beg]);
        __m256i subRes = _mm256_subs_epu16(clock, _subVal);
        _mm256_storeu_si256((__m256i*)&clocks[beg], subRes);
		beg += 16;
    }

    while(beg < end){
        if(clocks[beg] <= val)
            clocks[beg] = 0;
        else
            clocks[beg] -= val;
        beg++;
    }
}
