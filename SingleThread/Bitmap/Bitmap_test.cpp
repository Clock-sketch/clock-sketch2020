#include <chrono>
#include <iostream>
#include <random>
#include <unordered_set>
#include "Bitmap.h"
using namespace std::chrono;
using std::cout;
using std::endl;
using std::unordered_set;

void testBitmap_re(TRACE &trace, const int window_sz, const int memory, int clocksize, int hashnum)
{
    printf(">>> win:%d\tmem:%d\thashnum:%d\tclocksize:%d\n", window_sz, memory, hashnum, clocksize);
    Bitmap bm(window_sz, memory, hashnum, clocksize);

    double RE[10] = {0}, ARE = 0;
    for(int win_cnt = 0; win_cnt < 10; ++win_cnt)
    {
        unordered_set<int> inSet;
        for(int i = win_cnt * window_sz; i < (win_cnt + 1) * window_sz; ++i)
        {
            uint32_t ID = *(uint32_t*)(trace[i].key + 8);
            bm.insert(ID);
            bm.updateClock();
            inSet.insert(ID);
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
}

void testBitmap_th(TRACE &trace, const int window_sz, const int memory, int clocksize, int hashnum)
{
    printf(">>> win:%d\tmem:%d\thashnum:%d\tclocksize:%d\n", window_sz, memory, hashnum, clocksize);
    Bitmap bm(window_sz, memory, hashnum, clocksize);

    int insertTimesPerUpdate = 1;
    int packetCnt = trace.size() / insertTimesPerUpdate * insertTimesPerUpdate;
    uint32_t *packetIDs = new uint32_t[packetCnt];
    for(int i = 0; i < packetCnt; ++i)
        packetIDs[i] = *((uint32_t*)(trace[i].key + 8));
    
    int test_cycle = 1;
    for(int iCase = 0; iCase < 3; ++iCase){
        cout << "    iCase=" << iCase << ": ";
        auto t1 = steady_clock::now();
        for(int i = 0; i < test_cycle; ++i)
            // for(int k = 0; k < packetCnt; k += insertTimesPerUpdate){
            //     for(int j = 0; j < insertTimesPerUpdate; ++j)
            //         bm.insert(packetIDs[k + j]);
            //     bm.updateClock(insertTimesPerUpdate);
            // }
            for(int k = 0; k < packetCnt; k++){
                bm.insert(packetIDs[k]);
                bm.updateClock();
            }
        auto t2 = steady_clock::now();
        auto t3 = duration_cast<microseconds>(t2 - t1).count();
        cout << "(BloomFilter): throughput: " << packetCnt * test_cycle / (1.0 * t3) << " MIPs" << endl;
    }
    delete[] packetIDs;
}

