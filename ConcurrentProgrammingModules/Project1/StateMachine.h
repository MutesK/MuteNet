#pragma once

/*
	State -> State ���� -> ���� Condition

	State 
		DoWork, StateName
		
	State -> State ���̴� StateMachine�� �Ѵ�.

	StateMachine�� ��ϵ� State, Goto State, Now State�� ����.

	Goto State ��û�� ������ ���� State�� Return�� �Ǹ� ���̵ȴ�.

*/

#include "State.h"
#include "../DeadLock Checker/SafeSharedLock.h"
#include <map>

class StateMachine final
{
public:
	StateMachine();
	~StateMachine();

	void SetInitialState(const std::shared_ptr<State>& state);
	void RegisterState(const std::shared_ptr<State>& state);
	bool GotoState(const std::string& state);

	THREAD_SHUTDOWN_FLAG();

private:
	NON_COPYABLE(StateMachine);
	
private:
	void DoWork();
private:
	std::atomic_bool _isRunning = false;
	std::atomic<std::shared_ptr<State>> _nowState;
	std::map<std::string, std::shared_ptr<State>> _allState;
	
	std::unique_ptr<std::thread> _StateThread;
};

