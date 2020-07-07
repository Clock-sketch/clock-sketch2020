
class bitmap
{
public:

	static const int
		MAX_CELL_NUM = 32 * 1e5 + 5,
		MAX_HASH_NUM = 50;
	int
		window,
		memory,
		hash_num,
		width,
		
		countersize;
	int idx;
	BOBHash32 hash[MAX_HASH_NUM];
	BOBHash32 hash_time_offset;
	unsigned short bm[MAX_CELL_NUM];

	void init(int _window, int _memory, int _countersize = 2,int _hash_num = 1)
	{
		for (int i = 0; i < hash_num; i++)
		{
			hash[i].initialize(rand() % MAX_PRIME32);
		}
		hash_time_offset.initialize(rand() % MAX_PRIME32);

		window = _window;
		memory = _memory;
		hash_num = _hash_num;
		countersize = _countersize;
		width = memory / countersize;
		idx = 0;

	}
	void insert(int x)
	{
		for (int i = 0; i < hash_num; i++)
		{
			int pos = hash[i].run((char *)&x, sizeof(int)) % width;
			bm[pos] = (1 << countersize) - 1;
		}
	}
	void update(int time) {
		int temp = ((1 << countersize) - 1)*width/window;
		for (int j = 0; j < temp; j++, idx = (idx + 1) % width) {
			if (bm[idx] > 0) {
				bm[idx]--;
			}
		}
	}
	double query()
	{
		
		double u = 0;
		for (int i = 0; i < width; i++)
		{
			if (bm[i] == 0){
				u++;
			}
		}
		return -width * log(u / width);
	}
};

