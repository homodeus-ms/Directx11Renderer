#pragma once
#include <string>

using namespace std;

#ifndef OUT
#define OUT
#endif

class Utils
{
public:
	static bool IsStartWith(const std::string& str, const std::string & withThis);
	static bool IsStartWith(const std::wstring& wStr, const std::wstring& withThis);

	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	static string GetFilenameWithoutExt(const string& filenameWithExt);
	static wstring GetFilenameWithoutExt(const wstring& filenameWithExt);

	// TEMP : ascii only
	static wstring ToWString(string value);
	static string ToString(wstring value);
};

