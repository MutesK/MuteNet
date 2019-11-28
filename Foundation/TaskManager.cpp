#include "pch.h"
#include "TaskManager.h"
#include <random>

using namespace Util;
using namespace std;


TaskManager::TaskManager()
{
	_tasklist.clear();

	CreateAgent();
}

// TO DO
TaskManager::~TaskManager()
{
	_tasklist.clear();
}


void TaskManager::CreateAgent()
{
	for (uint32_t i = 0; i < std::thread::hardware_concurrency() * 4; i++)
	{
		auto agent = CreateAgent(i);
	}
}

bool TaskManager::AddTaskAgentid(Task* t, uint64_t agentNum)
{
	const auto agentName = "TaskAgent" + to_string(agentNum);

	return AddTaskAgentid(t, agentName);
}

bool TaskManager::AddTaskAgentid(Task* t, std::string agentName)
{
	const auto result = _tasklist.find(agentName);
	if (result == _tasklist.end())
		return false;

	auto agent = result->second;
	agent->AddTask(*t);
	return true;
}

bool TaskManager::AddTask(Task* t)
{
	auto tasksize = _tasklist.size();

	random_device rn;
	mt19937_64 rnd(rn());

	const uniform_int_distribution<int> range(0, static_cast<int>(_tasklist.size()));
	return AddTaskAgentid(t, range(rnd));
}

std::shared_ptr<TaskAgent> TaskManager::CreateAgent(uint64_t agentNum)
{
	const auto agentName = "TaskAgent" + to_string(agentNum);

	return CreateAgent(agentName);
}

std::shared_ptr<TaskAgent> TaskManager::CreateAgent(std::string agentName)
{
	auto Agent = make_shared<TaskAgent>(agentName);

	_tasklist[agentName] = Agent;
	return Agent;
}
