#pragma once
#include <memory>
#include <string>
using namespace std;

#include "Utils/Delegate.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnLogDelegate, const string&);

enum class LogLevel
{
	Log,
	Warning,
	Error,
};

#define LogLevel_Log LogLevel::Log
#define LogLevel_LOG LogLevel::Log
#define LogLevel_Warning LogLevel::Warning
#define LogLevel_WARNING LogLevel::Warning
#define LogLevel_Error LogLevel::Error
#define LogLevle_ERROR LogLevel::Error

extern OnLogDelegate g_OnLogDelegate;

void InitLogFile();
void LogMessage(LogLevel level, const char* file, int32 line, const char* func, const std::string& msg);

#define LOG(level, msg) LogMessage(LogLevel_##level, __FILE__, __LINE__, __func__, msg)