#include <chrono>
#include <iostream>
#include <random>
#include "BloomFilter.h"
using namespace std::chrono;
using std::cout;
using std::endl;

// unit of memory is byte
void testBloomFilter_fpr(TRACE &trace, const int window_sz, const int memory, int hashnum, int clocksize,
    int start_time, int end_time, int insertTimesPerQuery)
{
    printf("countersize:%d\twindowsz:%d\tmem:%dB:\t", clocksize, window_sz, memory);
    BloomFilter bf(window_sz, memory, hashnum, clocksize);
    std::default_random_engine rand_engine;

    int update_freq = 10;
    int TP = 0, FP = 0, FN = 0, TN = 0;
    for(int i = 0; i < end_time; ++i)
    {
        if(i >= start_time && (i - start_time) % insertTimesPerQuery == 0)
        {
            int q = rand_engine();
            bool ans = bf.query(q);
            bool trueans = false;
            for(int j = i - 1; j >= std::max(0, i - window_sz / 2 * 3); j--)
                if(memcmp(trace[j].key + 8, &q, sizeof(int)) == 0){
                    trueans = true;
                    break;
                }
            
            if(ans && trueans)          TP++;
            else if(ans && !trueans)    FP++;
            else if(!ans && trueans)    TN++;
            else                        FN++;
        }
        bf.insert(*(int*)(trace[i].key + 8));
        if(i % update_freq == 0)
            bf.updateClock(update_freq);
    }
    double FPR = double(FP) / (TP + FP + TN + FN);
    printf("TP=%d, FP=%d, TN=%d, FN=%d, FPR=%.6lf\n", TP, FP, TN, FN, FPR);
}

// unit of memory is byte
void testBloomFilter_th(TRACE &trace, const int window_sz, const int memory, int hashnum, int clocksize)
{
    printf(">>> win:%d\tmem:%d\thashnum:%d\tclocksize:%d\n", window_sz, memory, hashnum, clocksize);
    BloomFilter bf(window_sz, memory, hashnum, clocksize);

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
            //         bf.insert(packetIDs[k + j]);
            //     bf.updateClock(insertTimesPerUpdate);
            // }
            for(int k = 0; k < packetCnt; k++){
                bf.insert(packetIDs[k]);
                bf.updateClock();
            }
        auto t2 = steady_clock::now();
        auto t3 = duration_cast<microseconds>(t2 - t1).count();
        cout << "(BloomFilter): throughput: " << packetCnt * test_cycle / (1.0 * t3) << " MIPs" << endl;
    }
    delete[] packetIDs;
}
