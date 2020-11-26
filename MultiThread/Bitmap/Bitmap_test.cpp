#include <chrono>
#include <iostream>
#include <random>
#include <unordered_set>
#include "Bitmap.h"
using namespace std::chrono;
using std::cout;
using std::endl;
using std::unordered_set;

void testBitmap_re(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate, int hashnum)
{
    window_sz = window_sz / insertTimesPerUpdate * insertTimesPerUpdate;
    printf(">>> win:%d\tmem:%d\thashnum:%d\tclocksize:%d\n", window_sz, memory, hashnum, clocksize);
    Bitmap bm(window_sz, memory, hashnum, clocksize, insertTimesPerUpdate);

    int packetCnt = trace.size() / insertTimesPerUpdate * insertTimesPerUpdate;
    int32_t *packetIDs = new int32_t[packetCnt];
    for(int i = 0; i < packetCnt; ++i)
        packetIDs[i] = *((int32_t*)(trace[i].key + 8));

    double RE[10] = {0}, ARE = 0;
    for(int win_cnt = 0; win_cnt < 10; ++win_cnt)
    {
        unordered_set<int> inSet;
        for(int i = win_cnt * window_sz; i + insertTimesPerUpdate <= (win_cnt + 1) * window_sz; i += insertTimesPerUpdate)
        {
            for(int k = 0; k < insertTimesPerUpdate; ++k)
                inSet.insert(packetIDs[i + k]);
            bm.Insert(i, packetIDs);
        }
        double real_card = (double)inSet.size();
        double bm_card = bm.query();
        RE[win_cnt] = fabs(1 - bm_card / real_card);
        ARE += RE[win_cnt];
    }
    printf("Average-RE:%.6lf,\tRE:", ARE / 10);
    for(int i = 0; i < 10; ++i)
        printf(" %.6lf", RE[i]);
    printf("\n");
    delete[] packetIDs;
}

void testBitmap_th(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate, int hashnum)
{
    window_sz = window_sz / insertTimesPerUpdate * insertTimesPerUpdate;
    printf(">>> win:%d\tmem:%d\thashnum:%d\tclocksize:%d\n", window_sz, memory, hashnum, clocksize);
    Bitmap bm(window_sz, memory, hashnum, clocksize, insertTimesPerUpdate);

    int packetCnt = trace.size() / insertTimesPerUpdate * insertTimesPerUpdate;
    int32_t *packetIDs = new int32_t[packetCnt];
    for(int i = 0; i < packetCnt; ++i)
        packetIDs[i] = *((int32_t*)(trace[i].key + 8));
    
    int test_cycle = 1;
    for(int iCase = 0; iCase < 3; ++iCase){
        cout << "    iCase=" << iCase << ": ";
        auto t1 = steady_clock::now();
        for(int i = 0; i < test_cycle; ++i)
            for(int k = 0; k + insertTimesPerUpdate <= packetCnt; k += insertTimesPerUpdate)
                bm.Insert(k, packetIDs);
        auto t2 = steady_clock::now();
        auto t3 = duration_cast<microseconds>(t2 - t1).count();
        cout << "(BloomFilter): throughput: " << packetCnt * test_cycle / (1.0 * t3) << " MIPs" << endl;
    }
    delete[] packetIDs;
}

