CC=g++
INCLUDE = -I../include -I../
CFLAGS= -std=c++11
LIBS = -lpthread

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: http

http: $(OBJS)
	$(CC) $^ -c $(LIBS) $(CFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)

clean:
	rm $(OBJS)
