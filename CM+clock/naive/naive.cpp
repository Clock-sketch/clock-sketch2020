
//naive cm sketch with clock
#include<vector>
#include<iostream>
#include<unordered_map>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<deque>
#include<random>
#include<ctime>
#pragma warning(disable:4996)
#define TIME_STAMP uint64_t
#define ID uint32_t
#define END_FILE_NO 0
#define START_FILE_NO 0
#define KEY_LENGTH 8
#define ITEM_LENGTH 16
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a) ((a)>0?(a):(-(a)))
#define random(a,b) (rand()%(b-a)+a)
using namespace std;


struct se {
	int cnt;
	int lasttime;
	int time;
};

struct FIVE_TUPLE {
	char key[ITEM_LENGTH];
};
typedef vector<FIVE_TUPLE> TRACE;

typedef unordered_map<string, se> FREQ_RECORD;




#ifndef _BOBHASH32_H
#define _BOBHASH32_H
#include <cstdio>
#include <random>
#include <vector>
#include <unordered_set>


#define MAX_PRIME32 1229
#define MAX_BIG_PRIME32 50

class BOBHash32
{
public:
	BOBHash32();
	~BOBHash32();
	BOBHash32(uint32_t prime32Num);
	void initialize(uint32_t prime32Num);
	uint32_t run(const char * str, uint32_t len);	// produce a hash number
	static uint32_t get_random_prime_index()
	{
		random_device rd;
		return rd() % MAX_PRIME32;
	}

	static vector<uint32_t> get_random_prime_index_list(int n)
	{
		random_device rd;
		unordered_set<int> st;
		while (st.size() < n) {
			st.insert(rd() % MAX_PRIME32);
		}
		return vector<uint32_t>(st.begin(), st.end());
	}
private:
	uint32_t prime32Num;
};

uint32_t big_prime3232[MAX_BIG_PRIME32] = {
	20177, 20183, 20201, 20219, 20231, 20233, 20249, 20261, 20269, 20287,
	20297, 20323, 20327, 20333, 20341, 20347, 20353, 20357, 20359, 20369,
	20389, 20393, 20399, 20407, 20411, 20431, 20441, 20443, 20477, 20479,
	20483, 20507, 20509, 20521, 20533, 20543, 20549, 20551, 20563, 20593,
	20599, 20611, 20627, 20639, 20641, 20663, 20681, 20693, 20707, 20717
};
uint32_t prime32[MAX_PRIME32] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
	31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
	73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
	127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
	179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
	233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
	283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
	353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
	419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
	467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
	547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
	607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
	661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
	739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
	811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
	877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
	947, 953, 967, 971, 977, 983, 991, 997,
	1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061,
	1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123,
	1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213,
	1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283,
	1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361,
	1367, 1373, 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439,
	1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493,
	1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571,
	1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627,
	1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721,
	1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789,
	1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877,
	1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973,
	1979, 1987, 1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029,
	2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111,
	2113, 2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203,
	2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273,
	2281, 2287, 2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347,
	2351, 2357, 2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411,
	2417, 2423, 2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503,
	2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593,
	2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677,
	2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729,
	2731, 2741, 2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801,
	2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887,
	2897, 2903, 2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969,
	2971, 2999, 3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061,
	3067, 3079, 3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167,
	3169, 3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251,
	3253, 3257, 3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323,
	3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391,
	3407, 3413, 3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491,
	3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557,
	3559, 3571, 3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631,
	3637, 3643, 3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709,
	3719, 3727, 3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797,
	3803, 3821, 3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881,
	3889, 3907, 3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947,
	3967, 3989, 4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049,
	4051, 4057, 4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129,
	4133, 4139, 4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219,
	4229, 4231, 4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283,
	4289, 4297, 4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391,
	4397, 4409, 4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481,
	4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561,
	4567, 4583, 4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649,
	4651, 4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729,
	4733, 4751, 4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813,
	4817, 4831, 4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931,
	4933, 4937, 4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993,
	4999, 5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077,
	5081, 5087, 5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167,
	5171, 5179, 5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261,
	5273, 5279, 5281, 5297, 5303, 5309, 5323, 5333, 5347, 5351,
	5381, 5387, 5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437,
	5441, 5443, 5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507,
	5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591,
	5623, 5639, 5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683,
	5689, 5693, 5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779,
	5783, 5791, 5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849,
	5851, 5857, 5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923,
	5927, 5939, 5953, 5981, 5987, 6007, 6011, 6029, 6037, 6043,
	6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121,
	6131, 6133, 6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211,
	6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287,
	6299, 6301, 6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359,
	6361, 6367, 6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451,
	6469, 6473, 6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563,
	6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659,
	6661, 6673, 6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733,
	6737, 6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827,
	6829, 6833, 6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907,
	6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983,
	6991, 6997, 7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069,
	7079, 7103, 7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187,
	7193, 7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253,
	7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369,
	7393, 7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487,
	7489, 7499, 7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549,
	7559, 7561, 7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621,
	7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703,
	7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817,
	7823, 7829, 7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901,
	7907, 7919, 7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009,
	8011, 8017, 8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093,
	8101, 8111, 8117, 8123, 8147, 8161, 8167, 8171, 8179, 8191,
	8209, 8219, 8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273,
	8287, 8291, 8293, 8297, 8311, 8317, 8329, 8353, 8363, 8369,
	8377, 8387, 8389, 8419, 8423, 8429, 8431, 8443, 8447, 8461,
	8467, 8501, 8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573,
	8581, 8597, 8599, 8609, 8623, 8627, 8629, 8641, 8647, 8663,
	8669, 8677, 8681, 8689, 8693, 8699, 8707, 8713, 8719, 8731,
	8737, 8741, 8747, 8753, 8761, 8779, 8783, 8803, 8807, 8819,
	8821, 8831, 8837, 8839, 8849, 8861, 8863, 8867, 8887, 8893,
	8923, 8929, 8933, 8941, 8951, 8963, 8969, 8971, 8999, 9001,
	9007, 9011, 9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091,
	9103, 9109, 9127, 9133, 9137, 9151, 9157, 9161, 9173, 9181,
	9187, 9199, 9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277,
	9281, 9283, 9293, 9311, 9319, 9323, 9337, 9341, 9343, 9349,
	9371, 9377, 9391, 9397, 9403, 9413, 9419, 9421, 9431, 9433,
	9437, 9439, 9461, 9463, 9467, 9473, 9479, 9491, 9497, 9511,
	9521, 9533, 9539, 9547, 9551, 9587, 9601, 9613, 9619, 9623,
	9629, 9631, 9643, 9649, 9661, 9677, 9679, 9689, 9697, 9719,
	9721, 9733, 9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791,
	9803, 9811, 9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871,
	9883, 9887, 9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967,
	9973
};

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

BOBHash32::BOBHash32()
{
	this->prime32Num = 0;
}

BOBHash32::BOBHash32(uint32_t prime32Num)
{
	this->prime32Num = prime32Num;
}

void BOBHash32::initialize(uint32_t prime32Num)
{
	this->prime32Num = prime32Num;
}

uint32_t BOBHash32::run(const char * str, uint32_t len)
{
	//register ub4 a,b,c,len;
	uint32_t a, b, c;
	//	uint32_t initval = 0;
	/* Set up the internal state */
	//len = length;
	a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
	c = prime32[this->prime32Num];         /* the previous hash value */

										   /*---------------------------------------- handle most of the key */
	while (len >= 12)
	{
		a += (str[0] + ((uint32_t)str[1] << 8) + ((uint32_t)str[2] << 16) + ((uint32_t)str[3] << 24));
		b += (str[4] + ((uint32_t)str[5] << 8) + ((uint32_t)str[6] << 16) + ((uint32_t)str[7] << 24));
		c += (str[8] + ((uint32_t)str[9] << 8) + ((uint32_t)str[10] << 16) + ((uint32_t)str[11] << 24));
		mix(a, b, c);
		str += 12; len -= 12;
	}

	/*------------------------------------- handle the last 11 bytes */
	c += len;
	switch (len)              /* all the case statements fall through */
	{
	case 11: c += ((uint32_t)str[10] << 24);
		// fall through
	case 10: c += ((uint32_t)str[9] << 16);
		// fall through
	case 9: c += ((uint32_t)str[8] << 8);
		/* the first byte of c is reserved for the length */
		// fall through
	case 8: b += ((uint32_t)str[7] << 24);
		// fall through
	case 7: b += ((uint32_t)str[6] << 16);
		// fall through
	case 6: b += ((uint32_t)str[5] << 8);
		// fall through
	case 5: b += str[4];
		// fall through
	case 4: a += ((uint32_t)str[3] << 24);
		// fall through
	case 3: a += ((uint32_t)str[2] << 16);
		// fall through
	case 2: a += ((uint32_t)str[1] << 8);
		// fall through
	case 1: a += str[0];
		/* case 0: nothing left to add */
	}
	mix(a, b, c);
	/*-------------------------------------------- report the result */
	return c;
}

BOBHash32::~BOBHash32()
{

}
#endif //_BOBHASH32_H





#define counter_t unsigned short
#define CNT_MAX 1<<15
#define ALPHA 1
#define BETA 2

class cm_sketch {

public:
	static const int
		MAX_CELL_NUM = 1e7,
		MAX_HASH_NUM = 50;
	int

		memory,
		hash_num, //number of hash function
		max_cell_num,
		group_size;
	int current_time = 1;
	TIME_STAMP window; //window size
	BOBHash32* hash;
	counter_t* counter;
	TIME_STAMP* timestamp;
	cm_sketch(TIME_STAMP _window, int _memory,
		int _hash_num) {

		window = _window,
			memory = _memory,
			hash_num = _hash_num,
			max_cell_num = memory / (2 + 8);
		hash = new BOBHash32[hash_num];
		for (int i = 0; i < hash_num; i++)
		{
			hash[i].initialize(rand() % MAX_PRIME32);
		}
		counter = new counter_t[max_cell_num];
		timestamp = new TIME_STAMP[max_cell_num];
		for (int i = 0; i < max_cell_num; i++)
		{
			counter[i] = 0;
			timestamp[i] = 0;
		}

	}

	void insert(int key) {
		current_time++;
		for (int i = 0; i < hash_num; i++) {
			int index = hash[i].run((char*)&key, sizeof(int)) % max_cell_num;
			if (current_time - window <= timestamp[index])
				counter[index] += 1;
			else
				counter[index] = 1;
			timestamp[index] = current_time;
		}

	}



	int query(int key) {
		counter_t ret = CNT_MAX;
		for (int i = 0; i < hash_num;i++) {
			int index = hash[i].run((char*)&key, sizeof(int)) % max_cell_num;
			ret = MIN(ret, counter[index]);
			if (current_time - window > timestamp[index])
				return 0;
		}
		return (int)ret;
	}




	double calc_ARE(ID* stream, int start_packet, int end_packet, int freq) {


		double RE_sum = 0;
		int te = 0;
		double s = 0;
		double pack = 0;
		for (int i = 0; i < end_packet; i++) {

			insert(stream[i]);


			if (i >= start_packet && (i - start_packet) % freq == 0) {
				int q = stream[i];
				int ans = query(q);
				int j = i - 1;
				int last = i;
				int bagcnt = 1;
				for (; j >= 0; j--) {
					if (stream[j] == q)
					{
						last = j;
						bagcnt++;
					}
					if (last - j > window)
						break;
				}
				if (bagcnt >= 3) {
					te++;
					RE_sum += (double)ABS(ans - bagcnt) / bagcnt;
				}
			}


		}

		double tep = RE_sum / (double)te;
		printf("%d,%d,%.8f\n", window, memory, tep);
		return tep;


	}
};

TRACE traces[END_FILE_NO - START_FILE_NO + 1];
#define STREAMSIZE (1<<21)

void ReadInTraces() {
	for (int datafileCnt = START_FILE_NO; datafileCnt <= END_FILE_NO; datafileCnt++) {
		char datafileName[100];
		sprintf(datafileName, "./webdocs_form00.dat", datafileCnt);

		FILE *fin = fopen(datafileName, "rb");
		cout << datafileName << endl;

		FIVE_TUPLE tmp_five_tuple;
		traces[datafileCnt].clear();
		int datacnt = 0;
		while (fread(&tmp_five_tuple, 1, ITEM_LENGTH, fin) == ITEM_LENGTH && datacnt < STREAMSIZE) {
			traces[datafileCnt].push_back(tmp_five_tuple);
			datacnt++;
		}
		fclose(fin);
		printf("Successfully read in %s, %ld packets\n", datafileName, traces[datafileCnt].size());
	}
	printf("\n");
}
int test_trace(TRACE& trace, cm_sketch& CM, int start_time, int end_time, int freq)
{

	FREQ_RECORD real_freq;
	TIME_STAMP* time_stamp = new TIME_STAMP[end_time];
	ID* id = new ID[end_time];
	for (int cnt = 0; cnt < end_time; cnt++) {
		time_stamp[cnt] = *(TIME_STAMP*)(&trace[cnt]);
		id[cnt] = *((ID*)(&trace[cnt]) + 2) + *((ID*)(&trace[cnt]) + 3);
		//cout << time_stamp[cnt] << ' ' << id[cnt] << endl;
	}
	//clock_t starttime = clock();
	CM.calc_ARE(id, start_time, end_time, freq);

	//clock_t endtime = clock();
	//printf(",%f\n", (double)end_time / (endtime - starttime));
	delete[]time_stamp;
	delete[]id;
	return 0;
}

int main() {
	ReadInTraces();
	printf("window,memory,ARE\n");


    srand(clock());
	for (int i = START_FILE_NO; i <= END_FILE_NO; i++) {
		for (int count = 2; count <= 2; count*=2) {
			for (int win = (1 << 10); win <= (1 << 16); win <<= 2) {
				for (int mem = (1 << 15); mem <= (1 << 22); mem <<= 1) {
					cm_sketch CM=cm_sketch(win, mem/8, 8);
					test_trace(traces[i],CM,win*5+1,1<<21,1<<2);
				}
			}
		}
	}
	system("pause");
	return 0;
}





