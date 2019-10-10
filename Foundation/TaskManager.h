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
#include "Task.h"
#include "TaskAgent.h"

namespace Util
{
	class TaskAgent;
	class TaskManager final
	{
	public:
		TaskManager();
		~TaskManager();

		static TaskManager* Instance();

		bool AddTaskAgentid(Task* t, uint64_t agentNum);
		bool AddTaskAgentid(Task* t, std::string agentName);

		bool AddTask(Task* t);

		NON_COPYABLE(TaskManager);
	private:
		void CreateAgent();

		std::shared_ptr<TaskAgent> CreateAgent(uint64_t agentNum);
		std::shared_ptr<TaskAgent> CreateAgent(std::string agentName);

	private:
		static std::unique_ptr<TaskManager> _instance;
		static std::once_flag _flag;

		Concurrency::concurrent_unordered_map<std::string, std::shared_ptr<TaskAgent>> _tasklist;
	};



#define Enqueue_TASK(Dolambda)								\
{															\
	Util::Task task(Dolambda, __FUNCTION__, __FILE__, __LINE__);	\
	Util::TaskManager::Instance()->AddTask(&task);				\
}

#define Enqueue_Agent_Task(Dolambda, agentid)				\
{															\
	Util::Task task(Dolambda, __FUNCTION__, __FILE__, __LINE__);	\
	Util::TaskManager::Instance()->AddTaskAgentid(&task, agentid); \
}

#define Enqueue_COMPLETE_TASK(Dolamdba, CompleteLambda)		\
{															\
	Util::Task task(Dolamdba, CompleteLambda, __FUNCTION__,		\
	__FILE__, __LINE__);									\
	Util::TaskManager::Instance()->AddTask(&task);				\
}

#define Enqueue_Agent_COMPLETE_TASK(Dolamdba, CompleteLambda, agentid)		\
{																			\
	Util::Task task(Dolamdba, CompleteLambda, __FUNCTION__,						\
	__FILE__, __LINE__);													\
	Util::TaskManager::Instance()->AddTaskAgentid(&task, agentid);				\
}

}