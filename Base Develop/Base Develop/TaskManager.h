#pragma once

/*
	TaskManager

	사실상 Task와 TaskAgent를 관리한다.
	외부에서는 직접적으로 Task와 TaskAgent을 사용하지않고,
	TaskManager을 통해 사용한다.

	필요한 기능

	1. 원하는 Agent에 Task 추가하기
	2. 랜덤 Agent에 Task 추가하기

*/

class TaskAgent;
class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	static TaskManager* Instance();
private:
	std::shared_ptr<TaskAgent> CreateAgent(uint64_t agentNum);
	std::shared_ptr<TaskAgent> CreateAgent(std::string agentName);

private:
	static std::unique_ptr<TaskManager> _instance;
	static std::once_flag				_flag;
	std::unordered_map<std::string, std::shared_ptr<TaskAgent>> _tasklist;
};


