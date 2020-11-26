#include <stdio.h>
#include <stdlib.h>
#include "../CMSketch/CMSketch.h"
#include "../BloomFilter/BloomFilter.h"
#include "../Bitmap/Bitmap.h"
#include "utils.h"

TRACE traces[END_FILE_NO - START_FILE_NO + 1];

void testCM()
{
    int win = 1 << 14;
    int mem = 512 * 1024 * 8;
    int count = 8;
    int insertTimesPerUpdate = 10;

    testCMSketch_are(traces[0], win, mem, count, insertTimesPerUpdate);
    testCMSketch_th(traces[0], win, mem, count, insertTimesPerUpdate);
    printf("*********************************************\n\n");
}

void testBF()
{
    int query_freq = 10;
    int win = 1 << 12;
    int mem = 8 * 1024;
    int clocksize = 2;
    int hashnum = 1 + (0.6931 * mem * 8) / (win * clocksize);
    int insertTimesPerUpdate = 10;

    printf("********* testing BloomFilter fpr *********\n");
    testBloomFilter_fpr(traces[0], win, mem, hashnum, clocksize, 0, 1 << 21, query_freq, insertTimesPerUpdate);

    printf("********* testing BloomFilter throughput *********\n");
    testBloomFilter_th(traces[0], win, mem, hashnum, clocksize, insertTimesPerUpdate);
    
    printf("*********************************************\n\n");
}

void testBF_ts()
{
    int query_freq = 10;
    int win = 1 << 12;
    int mem = 32 * 1024;
    int clocksize = 8;
    int hashnum = 1 + (0.6931 * mem * 8) / (win * clocksize);
    int insertTimesPerUpdate = 10;

    printf("********* testing BloomFilter+ts fpr *********\n");
    testBloomFilter_fpr(traces[0], win, mem, hashnum, clocksize, 0, 1 << 21, query_freq, insertTimesPerUpdate);

    printf("********* testing BloomFilter+ts throughput *********\n");
    testBloomFilter_th(traces[0], win, mem, hashnum, clocksize, insertTimesPerUpdate);
    
    printf("*********************************************\n\n");
}

void testBM()
{
    int win = 1 << 14;
    int mem = 128 * 1024;
    int clocksize = 8;
    int insertTimesPerUpdate = 10;

    printf("********* testing Bitmap related error *********\n");
    testBitmap_re(traces[0], win, mem, clocksize, insertTimesPerUpdate);

    printf("********* testing Bitmap throughput *********\n");
    testBitmap_th(traces[0], win, mem, clocksize, insertTimesPerUpdate);
    
    printf("*********************************************\n\n");
}


int main()
{
    ReadTraces(traces);
    
    testCM();
    testBF();
    testBF_ts();
    testBM();

    exit(0);
}