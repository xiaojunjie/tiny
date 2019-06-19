include ../tiny.mk
current_dir = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

LIBS = -L../dist
LDFLAGS += -ltiny -lpthread -ldl
#INCLUDE += 

all: test config

test: main.o
	$(CC) $^ -o test_server $(LIBS)  $(LDFLAGS)
	rm -f *.o 

clean:
	rm -f *.o

.PHONY: config
config:
	@sed -i "/^log\s*=/ s:=.*:= $(current_dir)log/:" tiny.cfg
	@sed -i "/^assets\s*=/ s:=.*:= $(current_dir)assets/:" tiny.cfg
	@sed -i "/^template\s*=/ s:=.*:= $(current_dir)template/:" tiny.cfg
	@sed -i "/^port\s*=/ s:=.*:= 8888:" tiny.cfg
	@cat tiny.cfg

