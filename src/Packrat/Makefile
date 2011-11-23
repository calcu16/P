CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++0x
OBJECTS = match.o symbol.o parser.o

all: $(OBJECTS)

match.o: match.cpp match.hpp
symbol.o: symbol.cpp symbol.hpp parser.hpp match.hpp
parser.o: parser.cpp parser.hpp symbol.hpp symbol.hpp

clean:
	rm -f *.o
