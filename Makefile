include tiny.mk

LIB_HTTP_OBJS = http/http_message.o http/http_protocol.o
lIB_SOCKET_OBJS = socket/socket_stream.o
lIB_ROUTE_OBJS = route/route.o
LIB_FILE_OBJS = file/file_template.o file/file_logger.o
LIB_OBJS = $(lIB_ROUTE_OBJS) $(LIB_HTTP_OBJS) $(lIB_SOCKET_OBJS) $(LIB_FILE_OBJS)

all: libtiny.a

libtiny.a: tiny.o tool.o $(LIB_OBJS)
	@(mkdir -p ./dist)
	$(AR) ./dist/$@ $^ *.o -o 
	@(rm *.o)

.PHONY: example
example:
	@( cd example; make )

.PHONY: clean
clean:
	rm -f $(LIB_OBJS) *.o *.a dist/*.a

.PHONY: test
test:
	@(cd test; make; ./test_server tiny.cfg)

curl:
	curl 127.0.0.1:8888/ -v &
