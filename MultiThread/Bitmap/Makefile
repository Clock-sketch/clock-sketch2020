CFLAG = -std=c++17 -O2 -mavx2
CXX = g++
OBJS = Bitmap.o Bitmap_test.o

all: $(OBJS) move

Bitmap.o: Bitmap.cpp
	g++ -c Bitmap.cpp $(CFLAG)

Bitmap_test.o: Bitmap_test.cpp
	g++ -c Bitmap_test.cpp $(CFLAG)

move:
	cp $(OBJS) ../

clean:
	rm -rf $(OBJS)
