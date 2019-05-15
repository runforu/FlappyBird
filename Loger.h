#ifndef _LOGER_H_
#define _LOGER_H_

#include <string.h>

#if defined(_DEBUG)

class Loger {
public:
    static void out(int value);
    static void out(float value);
    static void out(const char* msg, ...);
};

#define LOG(format, ...) Loger::out(format, ##__VA_ARGS__);
#define LOG_INFO(info) Loger::out(info);
#define LOG_LINE Loger::out("Hit func =%s, line = %d ", __FUNCTION__, __LINE__);

class FuncWarder {
    char m_function_name[32];

public:
    FuncWarder(const char* name) {
        strncpy_s(m_function_name, sizeof(m_function_name), name, sizeof(m_function_name) - 1);
        LOG("------------------------------>>>     %s", m_function_name);
    }
    ~FuncWarder() {
        LOG("<<<------------------------------     %s", m_function_name);
    }
};

#define FUNC_WARDER FuncWarder $INVISIBLE(__FUNCTION__);

#else  // defined(_DEBUG)

#define LOG(format, ...)
#define LOG_INFO(inf)
#define LOG_LINE
#define FUNC_WARDER

#endif  // defined(_DEBUG)

#endif  // !_LOGER_H_
