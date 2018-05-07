#pragma once

#include "file_logstream.h"
namespace logger{
    extern logstream debug;
    extern logstream info;
    extern logstream warm;
    extern logstream error;
    extern logstream fatal;
    extern void init(std::string,Level);
    extern void destroy();
    template <typename T>
    extern T& endl(T&);
}
