CC=g++
CFLAGS= -std=c++0x -c -Wall -I/usr/include/python2.7 -I/usr/local/include/boost
LDFLAGS= -o -L/usr/local/lib
HEADERS= model.h ring_topo.h d_topo.h driver.h
SOURCES= model.cpp ring_topo.cpp d_topo.cpp driver.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=tman

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGET)

clobber:
	rm -f *.o