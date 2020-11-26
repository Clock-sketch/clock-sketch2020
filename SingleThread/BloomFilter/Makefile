CFLAG = -std=c++17 -O2 -mavx2
CXX = g++
OBJS = BloomFilter.o BloomFilter_test.o

all: $(OBJS) move

BloomFilter.o: BloomFilter.cpp
	g++ -c BloomFilter.cpp $(CFLAG)

BloomFilter_test.o: BloomFilter_test.cpp
	g++ -c BloomFilter_test.cpp $(CFLAG)

move:
	cp $(OBJS) ../

clean:
	rm -rf $(OBJS)
