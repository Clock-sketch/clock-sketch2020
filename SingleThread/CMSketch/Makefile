CFLAG = -std=c++17 -O2 -mavx2
CXX = g++
OBJS = CMSketch.o CMSketch_test.o

all: $(OBJS) move

CMSketch.o: CMSketch.cpp
	g++ -c CMSketch.cpp $(CFLAG)

CMSketch_test.o: CMSketch_test.cpp
	g++ -c CMSketch_test.cpp $(CFLAG)

move:
	cp $(OBJS) ../

clean:
	rm -rf $(OBJS)
