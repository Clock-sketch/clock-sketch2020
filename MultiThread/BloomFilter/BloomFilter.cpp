#include <math.h>
#include <immintrin.h> // AVX
#include "BloomFilter.h"

// unit of memory is byte
BloomFilter::BloomFilter(int _window, int _memory, int _hash_num, int _countersize, int _insertTimesPerUpdate)
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

    updator = thread(BloomFilter::updateThread, this);
}

BloomFilter::~BloomFilter()
{
    this->whileing = 0;
    updator.join();
    delete[] clocks;
    delete[] hash_func;
}

void BloomFilter::insert(int x)
{
    for(int i = 0; i < hash_num; ++i)
    {
        int pos = hash_func[i].run((char*)&x, sizeof(int)) % width;
        clocks[pos] = (1 << clocksize) - 1;
    }
}

void BloomFilter::Insert(int j, int flow[])
{
    this->updating = 1;
    this->blocking = 0;
    for(int k = j; k < j + insertTimesPerUpdate; ++k)
        this->insert(flow[k]);
    while(this->updating);
}

void BloomFilter::updateClock()
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

bool BloomFilter::query(int x)
{
    for(int i = 0; i < hash_num; ++i)
    {
        int pos = hash_func[i].run((char*)&x, sizeof(int)) % width;
        if(clocks[pos] == 0)
            return false;
    }
    return true;
}

void BloomFilter::updateRange(int beg, int end, int val)
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

void BloomFilter::updateThread(BloomFilter *bf)
{
    while(bf->whileing)
    {
        while(bf->whileing && bf->blocking);
        bf->updateClock();
        bf->blocking = 1;
        bf->updating = 0;
    }
}

