#include "pch.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <mutex>
#include <filesystem>

namespace fs = std::filesystem;

static std::ofstream logFile;
static std::mutex logMutex;

constexpr LogLevel g_minLogLevel =
#ifdef _DEBUG
	LogLevel::Log;
#else
	LogLevel::Error;  // ��������� Error�� ���
#endif

// ANSI Color Code
const char* GetLogColor(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Log:
		return "\033[32m";    // Green
	case LogLevel::Warning:
		return "\033[33m";    // Yellow
	case LogLevel::Error:
		return "\033[31m";    // Red
	default:
		return "\033[0m";
	}
}

const char* ToString(LogLevel level)
{
	switch (level)
	{
	case LogLevel::Log:
		return "Log";
	case LogLevel::Warning:
		return "Warning";
	case LogLevel::Error:
		return "Error";
	default:
		return "UNKNOWN";
	}
}

void InitLogFile()
{
	// ���� ���� ��� �������� ���� ���丮 ���ϱ�
	fs::path logDir = "C:/Users/seekc/OneDrive/Documents/Renderer/Renderer/Log";

	// Log ���丮 ���� (������)
	fs::create_directories(logDir);

	// �α� ���� ���
	fs::path logPath = logDir / "Log.txt";

	// ���� ����
	logFile.open(logPath, std::ios::trunc);
}

void LogMessage(LogLevel level, const char* file, int32 line, const char* func, const string& msg)
{
	if (level < g_minLogLevel)
		return;

	std::lock_guard<std::mutex> lock(logMutex);
	
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	std::tm localTime{};

#ifdef _WIN32
	localtime_s(&localTime, &now_c);
#else
	localtime_r(&now_c, &localTime);
#endif

	const string& fileName = std::filesystem::path(file).filename().string();

	std::ostringstream oss;
	oss << "[" << std::put_time(&localTime, "%H:%M:%S") << "] "
		<< "[" << ToString(level) << "] "
		<< msg
		<< "[" << fileName << ":" << line << "@" << func << "] ";

	std::string LogStr = oss.str();

	// �ܼ� ��� (���� ����)
	std::cout << GetLogColor(level) << LogStr << "\033[0m" << std::endl;

	// ���� ��� (���� ����)
	if (logFile.is_open()) {
		logFile << LogStr << std::endl;
	}
}
