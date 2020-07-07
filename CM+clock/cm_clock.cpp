
#define counter_t unsigned short

class cm_sketch {
private:
	int window_sz;
	int d;
	int width;
	counter_t** counters;
	unsigned short** clock;
	double idx;
	int id;
	int clocksize;
	BOBHash32** hash_func;

public:
	cm_sketch(int window_sz_, int d_, int width_, int _clocksize) :
		window_sz(window_sz_), d(d_), width(width_) {
		clocksize = _clocksize;
		idx = 0;
		id = 0;
		counters = new counter_t*[d];
		hash_func = new BOBHash32*[d];

		for (int i = 0; i < d; i++) {
			counters[i] = new counter_t[width];
			for (int j = 0; j < width; j++) {
				counters[i][j] = 0;
			}
			hash_func[i] = new BOBHash32(i * 12 + 221);
		}

		clock = new unsigned short*[d];
		for (int i = 0; i < d; i++) {
			clock[i] = new unsigned short[width];
			for (int j = 0; j < width; j++) {
				clock[i][j] = 0;
			}
		}
	}

	int getwindowsz() {
		return window_sz;
	}

	void insert(int time_cnt, const char* key, counter_t f, int clocksize) {

		for (int i = 0; i < d; i++) {
			int index = hash_func[i]->run(key, KEY_LENGTH) % width;
			counters[i][index] += f;
			clock[i][index] = (1 << clocksize) - 1;
		}
	}

	void updateclock(int time_cnt, int clocksize) {
		int ct=d*width;
		int temp = (1 << clocksize)*(ct)/window_sz - 1;
			for (int j = 0; j < temp; j++, idx = (idx + 1) % (ct)) {
                int d1=idx/width;
                int k1=idx%width;
                if(clock[d1][k1]>0){
                    clock[d1][k1]--;
                }
                else {
                    counters[d1][k1]=0;
                }
			}
	}

	int query(const char* key) {
		counter_t ret = CNT_MAX;
		for (int i = 0; i < d; i++) {
			int index = hash_func[i]->run(key , KEY_LENGTH) % width;
			ret = MIN(ret, counters[i][index]);
		}
		return (int)ret;
	}

	double calc_ARE(int time_cnt, const FREQ_RECORD& Real_Freq,int & shu) {
		double RE_sum = 0;
		int te = 0;
		double s=0;
		double pack=0;
		for (FREQ_RECORD::const_iterator it = Real_Freq.begin(); it != Real_Freq.end(); it++) {
			unsigned AE = ABS(it->second.cnt - query(it->first.c_str()));
			double RE = 0;
			if (it->second.cnt >=10) {
				RE=(double)AE/(double)it->second.cnt;
				te++;
        		RE_sum += RE;
				pack+=it->second.cnt;
				s+=AE;
			}
		}
		shu=te;
		double tep = RE_sum / (double)te;
		return tep;
	}
	 double getit(){
        int t=0;
        for(int i=0;i<d;i++){
            for(int j=0;j<width;j++){
                if(counters[i][j]==0){
                    t++;
                }
            }
        }
        return t/((double)width*d);
	 }

};