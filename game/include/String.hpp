#pragma once

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <string>
#include <locale>
#include <codecvt>

typedef std::wstring WString;
typedef std::string String;

inline std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

inline WString Widen(String from)
{
	return converter.from_bytes(from);
}

inline const wchar_t* WidenCStr(String from)
{
	return Widen(from).c_str();
}