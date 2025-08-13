#include "pch.h"
#include "SubWindowController.h"

void SubWindowController::BeginPlay()
{
	LOG(Log, "SubWindowController::BeginPlay()");
}

void SubWindowController::CreateSubWindow()
{
	static int count = 0;
	string s = to_string(count++);
	LOG(Log, "SubWindowController::BeginPlay()");
	LOG(Log, s);
}
