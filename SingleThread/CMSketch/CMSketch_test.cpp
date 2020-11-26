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
void testCMSketch_are(TRACE &trace, const int window_sz, const int memory, int clocksize)
{
    printf("********* testing CM sketch ARE *********\n");
    int time_cnt = 0;
    int window_num = 0;
    int d = 8;
    
    int width = memory / (d * (16 + clocksize));
    CMSketch cmsketch(window_sz, d, width, clocksize);

    double are = 0;
    for(int time_cnt = 1; time_cnt <= 10 * window_sz && window_num <= 8; time_cnt++)
    {
        string key1((const char*)(trace[time_cnt].key + 8), KEY_LENGTH);
        cmsketch.insert(time_cnt, key1.c_str(), 1, clocksize);
        cmsketch.updateClock();

        if(time_cnt >= 6 * window_sz && time_cnt % window_sz == 0)
        {
            FREQ_RECORD Real_Freq;
            findWindow(trace, time_cnt, Real_Freq, window_sz);
            double tep = cmsketch.calc_ARE(time_cnt, Real_Freq);
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
}

// unit of memory is bit
void testCMSketch_th(TRACE &trace, const int window_sz, const int memory, int clocksize)
{
    printf("********* testing CM sketch throughput *********\n");
    printf(">>> win:%d\tmem:%d\tclocksize:%d\n", window_sz, memory, clocksize);
    int d = 8;
    int width = memory / (d * (16 + clocksize));
    CMSketch cmsketch(window_sz, d, width, clocksize);

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
            //         cmsketch.insert(i * packetCnt + k + j, (char*)&packetIDs[k + j], 1, clocksize);
            //     cmsketch.updateClock(insertTimesPerUpdate);
            // }
            for(int k = 0; k < packetCnt; k++){
                cmsketch.insert(i * packetCnt + k, (char*)&packetIDs[k], 1, clocksize);
                cmsketch.updateClock();
            }
        auto t2 = steady_clock::now();
        auto t3 = duration_cast<microseconds>(t2 - t1).count();
        cout << "(CMSketch): throughput: " << packetCnt * test_cycle / (1.0 * t3) << " MIPs" << endl;
    }
    delete[] packetIDs;
}
