#include "pch.h"
#include "CommandQueue.h"

CommandQueue::CommandQueue()
{
}

CommandQueue::~CommandQueue()
{

}

void CommandQueue::PushCommand(Command command)
{
	_commands.push(command);
}

void CommandQueue::TickCommands()
{
	while (!_commands.empty())
	{
		_commands.front()();
		_commands.pop();
	}
}

