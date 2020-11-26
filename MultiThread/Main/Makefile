CFLAG = -std=c++17 -O2
CXX = g++
OBJS = BOBHash32.o utils.o main.o

all: $(OBJS) move

main.o: main.cpp
	$(CXX) -c main.cpp $(CFLAG)

utils.o: utils.cpp utils.h
	$(CXX) -c utils.cpp $(CFLAG)

BOBHash32.o: BOBHash32.cpp BOBHash32.h
	$(CXX) -c BOBHash32.cpp $(CFLAG)

move:
	mv $(OBJS) ../

clean:
	rm -rf $(OBJS)