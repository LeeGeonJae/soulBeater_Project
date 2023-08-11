#include "StringHelper.h"

#include <codecvt>
#include <locale>


namespace d2dFramework
{
	std::wstring StringHelper::string_to_wstring(const std::string& _str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(_str);
	}
}
