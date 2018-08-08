#include "stdafx.h"
#include "Thread.h"
#include "Task.h"
#include "TaskAgent.h"
#include "TaskManager.h"

using namespace std;

unique_ptr<TaskManager> TaskManager::_instance;
once_flag TaskManager::_flag;

TaskManager::TaskManager()
{
	_tasklist.clear();
}


TaskManager::~TaskManager()
{
}

TaskManager* TaskManager::Instance()
{
	std::call_once(_flag, 
		[&]()
	{
		_instance = std::make_unique<TaskManager>();
	});

	return _instance.get();
}

std::shared_ptr<TaskAgent> TaskManager::CreateAgent(uint64_t agentNum)
{
	std::string agentName = "TaskAgent" + agentNum;

	std::shared_ptr<TaskAgent> Agent = make_shared<TaskAgent>();

	Agent->SetThreadName(agentName);
	_tasklist[agentName] = Agent;

	return Agent;
}