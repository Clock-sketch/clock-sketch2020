#include <math.h>
#include <immintrin.h> // AVX
#include "Bitmap.h"

Bitmap::Bitmap(int _window, int _memory, int _hash_num, int _countersize, int _insertTimesPerUpdate)
    : window_sz(_window), hash_num(_hash_num), lastUpdateIdx(0), clocksize(_countersize),
    insertTimesPerUpdate(_insertTimesPerUpdate), updating(0), blocking(1), whileing(1)
{
    width = 8 * _memory / _countersize;
    updateLen = ((1 << _countersize) - 2) * width / _window;

    clocks = new Clock_t[width];
    memset(clocks, 0, sizeof(Clock_t) * width);

    hash_func = new BOBHash32[hash_num];
    for(int i = 0; i < hash_num; ++i)
        hash_func[i].initialize(100 + i);
    
    updator = thread(Bitmap::updateThread, this);
}

Bitmap::~Bitmap()
{
    this->whileing = 0;
    updator.join();
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

void Bitmap::Insert(int j, int flow[])
{
    this->updating = 1;
    this->blocking = 0;
    for(int k = j; k < j + insertTimesPerUpdate; ++k)
        this->insert(flow[k]);
    while(this->updating);
}

void Bitmap::updateClock()
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

void Bitmap::updateThread(Bitmap *bm)
{
    while(bm->whileing)
    {
        while(bm->whileing && bm->blocking);
        bm->updateClock();
        bm->blocking = 1;
        bm->updating = 0;
    }
}
