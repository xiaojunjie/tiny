#pragma once

#include "file_logstream.h"
#include "file_logboot.h"
namespace logger{
    extern LogBoot debug;
    extern LogBoot info;
    extern LogBoot warm;
    extern LogBoot error;
    extern LogBoot fatal;
    extern void init(std::string,Level);
    extern void destroy();
    template <typename T>
    extern T& endl(T&);
}
