#include <math.h>
#include <immintrin.h> // AVX
#include "CMSketch.h"

CMSketch::CMSketch(int window_sz_, int d_, int width_, int clockSize_)
    : window_sz(window_sz_), d(d_), width(width_), W(d_ * width_), lastUpdateIdx(0), clockSize(clockSize_)
{
    counters = new Counter_t[d * width];
    clocks = new Clock_t[d * width];
    memset(counters, 0, sizeof(Counter_t) * d * width);
    memset(clocks, 0, sizeof(Clock_t) * d * width);

    hash_func = new BOBHash32[d];
    for(int i = 0; i < d; ++i)
        hash_func[i].initialize(i * 12 + 221);
}

CMSketch::~CMSketch()
{
    delete[] counters;
    delete[] clocks;
    delete[] hash_func;
}

int CMSketch::getWindowSz()
{
    return window_sz;
}

void CMSketch::insert(int time_cnt, const char *key, Counter_t f, int clockSz)
{
    for(int i = 0; i < d; ++i)
    {
        int idx = (hash_func[i].run(key, KEY_LENGTH) % width) + width * i;
        counters[idx] += f;
        clocks[idx] = (1 << clockSize) - 1;
    }
}

void CMSketch::updateClock(int insertTimesPerUpdate)
{
    int temp = ((1 << clockSize) - 2) * W / window_sz * insertTimesPerUpdate;
    int subAll = temp / W;
    int len = temp % W;

    int beg = lastUpdateIdx, end = std::min(W, lastUpdateIdx + len);
    updateRange(beg, end, subAll + 1);
    if(end - beg < len)
    {
        end = len - (end - beg);
        beg = 0;
        updateRange(beg, end, subAll + 1);
    }
    
    if(end > lastUpdateIdx){
        updateRange(end, W, subAll);
        updateRange(0, lastUpdateIdx, subAll);
    }
    else
        updateRange(end, lastUpdateIdx, subAll);
    lastUpdateIdx = end;
}

int CMSketch::query(const char *key)
{
    Counter_t ret = 0xFFFF;
    for(int i = 0; i < d; ++i)
    {
        int idx = (hash_func[i].run(key, KEY_LENGTH) % width) + width * i;
        ret = std::min(ret, counters[idx]);
    }
    return (int)ret;
}

double CMSketch::calc_ARE(int time_cnt, const FREQ_RECORD &Real_Freq)
{
    double RE_sum = 0;
    int te = 0;
    for(auto it : Real_Freq)
    {
        unsigned AE = std::abs(it.second.cnt - this->query(it.first.c_str()));
        RE_sum += double(AE) / (double)(it.second.cnt);
        te++;
    }
    double tep = RE_sum / double(te);
    return tep;
}

void CMSketch::updateRange(int beg, int end, int val)
{
    if(val <= 0)    return;

    __m256i _subVal = _mm256_set1_epi16(short(val));
    while(beg + 16 <= end)
    {
        __m256i clock = _mm256_loadu_si256((__m256i*)&clocks[beg]);
        __m256i subRes = _mm256_subs_epu16(clock, _subVal);
        _mm256_storeu_si256((__m256i*)&clocks[beg], subRes);

        __m256i counter = _mm256_loadu_si256((__m256i*)&counters[beg]);
        __m256i x = _mm256_max_epu16(clock, _subVal);
        __m256i cmpeq = _mm256_cmpeq_epi16(x, _subVal);
        __m256i resAndnot = _mm256_andnot_si256(cmpeq, counter);

        _mm256_storeu_si256((__m256i*)&counters[beg], resAndnot);
		beg += 16;
    }

    while(beg < end){
        if(clocks[beg] <= val){
            clocks[beg] = 0;
            counters[beg] = 0;
        }
        else
            clocks[beg] -= val;
        beg++;
    }
}

