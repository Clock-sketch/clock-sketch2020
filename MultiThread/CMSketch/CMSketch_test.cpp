#include <chrono>
#include <iostream>
#include "CMSketch.h"
using namespace std::chrono;
using std::cout;
using std::endl;

void findWindow(const TRACE &trace, int t, FREQ_RECORD &acc, int win)
{
    acc.clear();
    for(int i = 0; i < win; ++i)
    {
        string key((const char*)(trace[t - i].key + 8), KEY_LENGTH);
        if(acc.count(key) <= 0)
        {
            acc[key].cnt = 1;
            acc[key].time = t - i;
            for(int j = t - i - 1; j >= 0 && j + win > acc[key].time; j--)
            {
                string key2((const char*)(trace[j].key + 8), KEY_LENGTH);
                if(key2 == key){
                    acc[key].cnt++;
                    acc[key].time = j;
                }
            }
        }
    }
}

// unit of memory is bit
void testCMSketch_are(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate)
{
    window_sz = window_sz / insertTimesPerUpdate * insertTimesPerUpdate;
    printf("********* testing CM sketch ARE *********\n");
    int time_cnt = 0;
    int window_num = 0;
    int d = 8;
    int width = memory / (d * (16 + clocksize));
    CMSketch cmsketch(window_sz, d, width, clocksize, insertTimesPerUpdate);
    
    int packetCnt = trace.size() / insertTimesPerUpdate * insertTimesPerUpdate;
    int32_t *packetIDs = new int32_t[packetCnt];
    for(int i = 0; i < packetCnt; ++i)
        packetIDs[i] = *((int32_t*)(trace[i].key + 8));

    double are = 0;
    for(int time_cnt = 0; time_cnt + insertTimesPerUpdate <= 10 * window_sz && window_num <= 8; time_cnt += insertTimesPerUpdate)
    {
        cmsketch.Insert(time_cnt, packetIDs);
        if((time_cnt + 10) >= 6 * window_sz && (time_cnt + 10) % window_sz == 0)
        {
            FREQ_RECORD Real_Freq;
            findWindow(trace, time_cnt + 10, Real_Freq, window_sz);
            double tep = cmsketch.calc_ARE(time_cnt + 10, Real_Freq);
            are += tep;

            printf("the temp countsize: %d\twindowsz:%d\tmem:%dB\tare:%lf\n",
                clocksize, window_sz, memory >> 3, tep);
            window_num++;
        }
    }
    printf(">>> countsize:%d\t windowsz:%d\t mem:%d\t are:%lf\n",
		clocksize,
		window_sz,
		memory >> 3,
		are / (window_num));
    delete[] packetIDs;
}

// unit of memory is bit
void testCMSketch_th(TRACE &trace, int window_sz, const int memory, int clocksize, int insertTimesPerUpdate)
{
    window_sz = window_sz / insertTimesPerUpdate * insertTimesPerUpdate;
    printf("********* testing CM sketch throughput *********\n");
    printf(">>> win:%d\tmem:%d\tclocksize:%d\n", window_sz, memory, clocksize);
    int d = 8;
    int width = memory / (d * (16 + clocksize));
    CMSketch cmsketch(window_sz, d, width, clocksize, insertTimesPerUpdate);

    int packetCnt = trace.size() / insertTimesPerUpdate * insertTimesPerUpdate;
    int32_t *packetIDs = new int32_t[packetCnt];
    for(int i = 0; i < packetCnt; ++i)
        packetIDs[i] = *((int32_t*)(trace[i].key + 8));
    
    int test_cycle = 1;
    for(int iCase = 0; iCase < 3; ++iCase){
        cout << "    iCase=" << iCase << ": ";
        auto t1 = steady_clock::now();
        for(int i = 0; i < test_cycle; ++i)
            for(int k = 0; k < packetCnt; k += insertTimesPerUpdate)
                cmsketch.Insert(k, packetIDs);            
        auto t2 = steady_clock::now();
        auto t3 = duration_cast<microseconds>(t2 - t1).count();
        cout << "(CMSketch): throughput: " << packetCnt * test_cycle / (1.0 * t3) << " MIPs" << endl;
    }
    delete[] packetIDs;
}
