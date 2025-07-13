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

void Utils::Replace(OUT string& str, string comp, string rep)
{
	string temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

void Utils::Replace(OUT wstring& str, wstring comp, wstring rep)
{
	wstring temp = str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	str = temp;
}

string Utils::GetFilenameWithoutExt(const string& filenameWithExt)
{
	size_t pos = filenameWithExt.rfind(L'.');
	return (pos == wstring::npos) ? filenameWithExt : filenameWithExt.substr(0, pos);
}

wstring Utils::GetFilenameWithoutExt(const wstring& filenameWithExt)
{
	size_t pos = filenameWithExt.rfind(L'.');
	return (pos == wstring::npos) ? filenameWithExt : filenameWithExt.substr(0, pos);
}

std::wstring Utils::ToWString(string value)
{
	return wstring(value.begin(), value.end());
}

std::string Utils::ToString(wstring value)
{
	return string(value.begin(), value.end());
}
