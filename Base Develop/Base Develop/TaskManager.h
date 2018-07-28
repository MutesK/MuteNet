#pragma once



class TaskAgent;
class TaskManager
{
public:
	TaskManager();
	~TaskManager();

	void Open(__int64 agentNum);
	uint64_t Run();


private:
	std::shared_ptr<TaskAgent> CreateAgent(uint64_t agentNum);
	std::shared_ptr<TaskAgent> CreateAgent(std::string agentName);

private:
	static std::unique_ptr<TaskManager> _instance;
};

