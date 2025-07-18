#pragma once
#include <queue>

using Command = std::function<void()>;

class CommandQueue
{
public:
	CommandQueue();
	~CommandQueue();

	void PushCommand(Command command);
	void TickCommands();

private:
	std::queue<Command> _commands;
};

