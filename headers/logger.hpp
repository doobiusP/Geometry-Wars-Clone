#pragma once
#include <iostream>

#ifdef DEBUG
#define LOG(x) x
#else
#define LOG(x) \
    do         \
    {          \
    } while (0)
#endif