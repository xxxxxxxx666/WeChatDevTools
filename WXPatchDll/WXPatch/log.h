#pragma once
#include <Windows.h>
#include <stdarg.h>
#include <string>


namespace Utils {

void output_debug_message_format_string(const char* fmt, ...);

void output_debug_message_format_wstring(const wchar_t* fmt, ...);

} // namespace Utils


#define XSTRING
#define XSTRING_IMM
#define ENABLE_LOG
#ifdef ENABLE_LOG

#define DBGHEADW                            L"<< Debug << "
#define DBGHEAD                             "<< Debug << "

#define D(fmt, ...)                         Utils::output_debug_message_format_string(XSTRING_IMM(DBGHEAD##"[+]"##fmt), __VA_ARGS__)
#define E(fmt, ...)                         _E(fmt,XSTRING(__FILE__), XSTRING(__FUNCTION__), __LINE__, __VA_ARGS__)
#define _E(fmt, file, func, line, ...)      Utils::output_debug_message_format_string(XSTRING_IMM(DBGHEAD##"[-]%s(%d)[%s] : "##fmt), strrchr(file, '\\')+1, line, func, __VA_ARGS__)
#define DW(fmt, ...)                        Utils::output_debug_message_format_wstring(DBGHEADW##"[+]"##fmt, __VA_ARGS__)

#else

#define D(fmt, ...)
#define E(fmt, ...)
#define DW(fmt, ...)

#endif // ENABLE_LOG
