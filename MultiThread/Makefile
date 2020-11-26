

CFLAG = -std=c++17 -O2
CXX = g++
dirs = $(shell ls -d */) 
OBJS = BOBHash32.o utils.o main.o \
	CMSketch.o CMSketch_test.o \
	BloomFilter.o BloomFilter_test.o \
	Bitmap.o Bitmap_test.o

all: test

finddirs:
	$(foreach N,$(dirs),make -C $(N);)

dclean:
	$(foreach N,$(dirs),make -C $(N) clean;) 

test: finddirs
	$(CXX) -o test $(OBJS) $(CFLAG)

clean: dclean
	rm -rf $(OBJS) test