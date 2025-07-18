#pragma once

class LogWindowController : public enable_shared_from_this<LogWindowController>
{
public:
	LogWindowController();
	~LogWindowController();

	void BeginPlay();
	void LogCallback(const string& log);

	void CreateDebugWindow();

private:
	vector<string> _logs;
};

