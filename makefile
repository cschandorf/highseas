#Define for the compilers
CC = gcc
CXX = g++

#compiler flags
#-g adds debugging information to the executable
#-Wall turn on most but not all compiler warnings
CXXFLAGS = -g -Wall -Wno-write-strings
#CXXLIBFLAGS = -fPIC -g -m32 -Wall -Wno-write-strings -Wno-missing-braces 

#the build target executable
TARGET = battleship

all: $(TARGET)

#building battleship
battleship: main.o error.o utils.o vessel.o gameboard.o
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c main.cpp

gameboard.o: gameboard.cpp gameboard.h error.h
	$(CXX) $(CXXFLAGS) -c gameboard.cpp

vessel.o: vessel.cpp vessel.h
	$(CXX) $(CXXFLAGS) -c vessel.cpp

utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp

error.o: error.cpp error.h
	$(CXX) $(CXXFLAGS) -c error.cpp

clean:
	rm -f *.o
	if [ -e "battleship" ]; then rm battleship; fi
