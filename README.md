# Tiny Web Server

## Introduction  
C++11编写的轻量级Web服务器，基于事件驱动实现了线程的异步唤醒，使用状态机解析了get、head请求，可处理静态资源，并实现了异步日志，无任何第三方库。

## Envoirment  
* OS: Ubuntu 16  
* Complier: g++ 4.8  
  
## Build  
make

## Usage  
 ./tiny configfile  

## Code  
.  
├── file  
|   ├── file_logger.cpp  
|   └── tiny_file_io.cpp  
├── http  
|   └── http_protocol.cpp  
├── os  
|   └── tiny_threadpool.cpp  
├── route  
|   ├── tiny_handler.cpp  
|   └── tiny_router.cpp  
├── socket  
|   ├── tiny_epoll.cpp  
|   ├── tiny_socket_io.cpp  
|   └── tiny_socket_stream.cpp  
└── tiny_core.cpp  
