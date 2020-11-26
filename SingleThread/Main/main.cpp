#include <stdio.h>
#include <stdlib.h>
#include "../CMSketch/CMSketch.h"
#include "../BloomFilter/BloomFilter.h"
#include "../Bitmap/Bitmap.h"
#include "utils.h"

TRACE traces[END_FILE_NO - START_FILE_NO + 1];

void testCM()
{
    // for(int i = START_FILE_NO; i <= END_FILE_NO; ++i)
    //     for(int count = 8; count <= 8; ++count)
    //         for(int win = (1 << 12); win <= (1 << 12); win <<= 2)
    //             for(int mem = (1 << 19); mem <= (1 << 19); mem <<= 1)
    //                 testCMSketch_are(traces[i], win, mem, count);

    // for(int i = START_FILE_NO; i <= END_FILE_NO; ++i)
    //     for(int count = 8; count <= 8; ++count)
    //         for(int win = (1 << 12); win <= (1 << 12); win <<= 2)
    //             for(int mem = (1 << 19); mem <= (1 << 19); mem <<= 1)
    //                 testCMSketch_th(traces[i], win, mem, count);
    int win = 1 << 14;
    int mem = 512 * 1024 * 8;
    int count = 8;
    testCMSketch_are(traces[0], win, mem, count);
    testCMSketch_th(traces[0], win, mem, count);
    printf("*********************************************\n\n");
}

void testBF()
{
    printf("********* testing BloomFilter fpr *********\n");
    int query_freq = 10;
    // for(int i = START_FILE_NO; i <= END_FILE_NO; ++i)
    //     for(int win = 1 << 16; win <= 1 << 16; win <<= 1)
    //         for(int mem = 8 * 1024; mem <= 256 * 1024; mem *= 2)
    //             for(int clocksize = 2; clocksize <= 2; clocksize *= 2)
    //                 for(int start = 0; start <= 0; start += win)
    //                 {
    //                     int hashnum = 1 + (0.6931 * mem * 8) / (win * clocksize);
    //                     testBloomFilter_fpr(traces[i], win, mem, hashnum, clocksize,
    //                                         start, 1<<21, query_freq);
    //                 }
    int win = 1 << 12;
    int mem = 8 * 1024;
    int clocksize = 2;
    int hashnum = 1 + (0.6931 * mem * 8) / (win * clocksize);
    testBloomFilter_fpr(traces[0], win, mem, hashnum, clocksize, 0, 1 << 21, query_freq);

    printf("********* testing BloomFilter throughput *********\n");
    // for(int i = START_FILE_NO; i <= END_FILE_NO; ++i)
    //     for(int win = 1 << 16; win <= 1 << 16; win <<= 1)
    //         for(int mem = 8 * 1024; mem <= 256 * 1024; mem *= 2)
    //             for(int clocksize = 2; clocksize <= 2; clocksize *= 2)
    //                 for(int start = 0; start <= 0; start += win)
    //                 {
    //                     int hashnum = 1 + (0.6931 * mem * 8) / (win * clocksize);
    //                     testBloomFilter_th(traces[i], win, mem, hashnum, clocksize);
    //                 }
    testBloomFilter_th(traces[0], win, mem, hashnum, clocksize);
    
    printf("*********************************************\n\n");
}

void testBF_ts()
{
    printf("********* testing BloomFilter+ts fpr *********\n");
    int query_freq = 10;
    int win = 1 << 12;
    int mem = 32 * 1024;
    int clocksize = 8;
    int hashnum = 1 + (0.6931 * mem * 8) / (win * clocksize);
    testBloomFilter_fpr(traces[0], win, mem, hashnum, clocksize, 0, 1 << 21, query_freq);

    printf("********* testing BloomFilter+ts throughput *********\n");
    testBloomFilter_th(traces[0], win, mem, hashnum, clocksize);
    
    printf("*********************************************\n\n");
}

void testBM()
{
    printf("********* testing Bitmap related error *********\n");
    // for(int i = START_FILE_NO; i <= END_FILE_NO; ++i)
    //     for(int win = 1 << 12; win <= 1 << 14; win <<= 1)
    //         for(int mem = 1 << 16; mem <= 1 << 16; mem <<= 1)
    //             for(int clocksize = 8; clocksize <= 8; clocksize *= 2)
    //                 testBitmap_re(traces[i], win, mem, clocksize);
    int win = 1 << 14;
    int mem = 128 * 1024;
    int clocksize = 8;
    testBitmap_re(traces[0], win, mem, clocksize);

    printf("********* testing Bitmap throughput *********\n");
    // for(int i = START_FILE_NO; i <= END_FILE_NO; ++i)
    //     for(int win = 1 << 12; win <= 1 << 14; win <<= 1)
    //         for(int mem = 1 << 16; mem <= 1 << 16; mem <<= 1)
    //             for(int clocksize = 8; clocksize <= 8; clocksize *= 2)
    //                 testBitmap_th(traces[i], win, mem, clocksize);
    testBitmap_th(traces[0], win, mem, clocksize);
    
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