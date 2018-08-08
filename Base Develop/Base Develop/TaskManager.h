#pragma once

/*
	TaskManager

	��ǻ� Task�� TaskAgent�� �����Ѵ�.
	�ܺο����� ���������� Task�� TaskAgent�� ��������ʰ�,
	TaskManager�� ���� ����Ѵ�.

	�ʿ��� ���

	1. ���ϴ� Agent�� Task �߰��ϱ�
	2. ���� Agent�� Task �߰��ϱ�

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


