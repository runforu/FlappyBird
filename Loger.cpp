#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "Loger.h"

#if defined(_DEBUG)

void Loger::out(int value) {
    Loger::out("%d", value);
}

void Loger::out(float value) {
    Loger::out("%f", value);
}

void Loger::out(const char* msg, ...) {
    char buffer[1024];
    va_list arg_ptr;
    va_start(arg_ptr, msg);
    _vsnprintf_s(buffer, sizeof(buffer) - 1, msg, arg_ptr);
    va_end(arg_ptr);

    printf(buffer);
    printf("\n");
}

#endif
