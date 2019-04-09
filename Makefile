include tiny.mk

LIB_TOOL_OBJS = tool.o
LIB_HTTP_OBJS = http/http_protocol.o
lIB_SOCKET_OBJS = socket/tiny_socket_stream.o socket/tiny_socket_io.o socket/tiny_epoll.o
lIB_ROUTE_OBJS = route/tiny_router.o
lIB_HANDLER_OBJS = route/tiny_handler.o
LIB_FILE_OBJS = file/tiny_file_io.o file/file_logger.o
LIB_OBJS = $(LIB_TOOL_OBJS) $(lIB_HANDLER_OBJS) $(lIB_ROUTE_OBJS) $(LIB_HTTP_OBJS) $(lIB_SOCKET_OBJS) $(LIB_FILE_OBJS)

all: libtiny.a

libtiny.a: tiny_core.o $(LIB_OBJS)
	@(mkdir -p ./dist)
	$(AR) ./dist/$@ $^ *.o -o 
	@(rm *.o)

.PHONY: example
example:
	@(clear)
	@( cd example; make && ./example_server tiny.cfg )

.PHONY: clean
clean:
	rm -f $(LIB_OBJS) *.o *.a dist/*.a

.PHONY: test
test:
	@(cd test; make && ./test_server tiny.cfg)

curl:
	curl 127.0.0.1:8888/ -v &
#
# 1 main thread push listenid to queue
# 2 worker get listenid and create a socket based on it
# 3 worker epoll_wait sockets until empty or all sleep, which need to be developed
# 4 back to step 2
# 
