#include "log.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma warning(disable:4996)
#endif

namespace Utils {

#define DEBUGOUT_STRING_BUFFER_SIZE (0x1000)

void output_debug_message_string(std::string str)
{
	OutputDebugStringA(str.c_str());
}

void output_debug_message_wstring(std::wstring str)
{
	OutputDebugStringW(str.c_str());
}

void output_debug_message_format_string(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);
	char* debugout_string_buffer = new char[DEBUGOUT_STRING_BUFFER_SIZE];
	if (debugout_string_buffer == NULL)
	{
		return;
	}
	vsprintf(debugout_string_buffer, format, vl);
	OutputDebugStringA(debugout_string_buffer);
	delete[] debugout_string_buffer;
	va_end(vl);
}

void output_debug_message_format_wstring(const wchar_t* format, ...)
{
	va_list vl;
	va_start(vl, format);
	wchar_t* debugout_string_buffer = new wchar_t[DEBUGOUT_STRING_BUFFER_SIZE];
	if (debugout_string_buffer == NULL)
	{
		return;
	}
	vswprintf(debugout_string_buffer, format, vl);
	OutputDebugStringW(debugout_string_buffer);
	delete[] debugout_string_buffer;
	va_end(vl);
}

} // namespace Utils
