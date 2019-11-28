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
#include "Singleton.h"
#include "Task.h"
#include "TaskAgent.h"

namespace Util
{
	class TaskAgent;
	class TaskManager final : public TL::Singleton<TaskManager>
	{
	private:
		Concurrency::concurrent_unordered_map<std::string, std::shared_ptr<TaskAgent>> _tasklist;

		friend class TL::Singleton<TaskManager>;
	public:
		~TaskManager();

		bool AddTaskAgentid(Task* t, uint64_t agentNum);
		bool AddTaskAgentid(Task* t, std::string agentName);

		bool AddTask(Task* t);

		NON_COPYABLE(TaskManager);
	protected:
		TaskManager();

	private:
		void CreateAgent();

		std::shared_ptr<TaskAgent> CreateAgent(uint64_t agentNum);
		std::shared_ptr<TaskAgent> CreateAgent(std::string agentName);
	};



#define Enqueue_TASK(Dolambda)								\
{															\
	Util::Task task(Dolambda, __FUNCTION__, __FILE__, __LINE__);	\
	Util::TaskManager::Get().AddTask(&task);				\
}

#define Enqueue_Agent_Task(Dolambda, agentid)				\
{															\
	Util::Task task(Dolambda, __FUNCTION__, __FILE__, __LINE__);	\
	Util::TaskManager::Get().AddTaskAgentid(&task, agentid); \
}

#define Enqueue_COMPLETE_TASK(Dolamdba, CompleteLambda)		\
{															\
	Util::Task task(Dolamdba, CompleteLambda, __FUNCTION__,		\
	__FILE__, __LINE__);									\
	Util::TaskManager::Get().AddTask(&task);				\
}

#define Enqueue_Agent_COMPLETE_TASK(Dolamdba, CompleteLambda, agentid)		\
{																			\
	Util::Task task(Dolamdba, CompleteLambda, __FUNCTION__,						\
	__FILE__, __LINE__);													\
	Util::TaskManager::Get().AddTaskAgentid(&task, agentid);				\
}

}