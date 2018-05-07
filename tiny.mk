where-am-i = $(abspath $(word $(words $(MAKEFILE_LIST)), $(MAKEFILE_LIST)))
TINY_ROOT = $(dir $(call where-am-i))

CC = g++
AR = ar crs
CFLAGS = -std=c++11 -Wall 
INCLUDE = -I./ -I$(TINY_ROOT) -I$(TINY_ROOT)include/
LDFLAGS = -lpthread

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ 
