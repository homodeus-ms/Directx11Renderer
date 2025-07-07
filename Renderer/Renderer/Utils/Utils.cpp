#include "Utils.h"

bool Utils::IsStartWith(const std::string& str, const std::string& withThis)
{
	std::string::size_type index = str.find(withThis);
	return index != std::string::npos && index == 0;
}

bool Utils::IsStartWith(const std::wstring& wStr, const std::wstring& withThis)
{
	std::wstring::size_type index = wStr.find(withThis);
	return index != std::wstring::npos && index == 0;
}
