#include "TaskManager.h"
#include <random>

using namespace Util;
using namespace std;

unique_ptr<TaskManager> TaskManager::_instance;
once_flag TaskManager::_flag;

TaskManager::TaskManager()
{
	_tasklist.clear();

	CreateAgent();
}

TaskManager::~TaskManager()
{
	for(const auto taskpair : _tasklist)
	{
		taskpair.second->SetShutdownThreadSignal(true);
	}

	_tasklist.clear();
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

void TaskManager::CreateAgent()
{
	// 64개의 에이전트 생성

	for (auto i = 0; i < 20; i++)
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
