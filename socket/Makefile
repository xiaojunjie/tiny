CC=g++
INCLUDE = -I../
CFLAGS= -std=c++11
LIBS = -lpthread

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: socket

socket: $(OBJS)
	$(CC) $^ -o tiny $(LIBS) $(CFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)

clean:
	rm $(OBJS)
