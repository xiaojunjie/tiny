#pragma once

#include "file_logstream.h"
namespace logger{
    extern logboot debug;
    extern logboot info;
    extern logboot warm;
    extern logboot error;
    extern logboot fatal;
    extern void init(std::string,Level);
    extern void destroy();
    template <typename T>
    extern T& endl(T&);
}
