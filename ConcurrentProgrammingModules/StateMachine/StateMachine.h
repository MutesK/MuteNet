#pragma once

/*
	State -> State 전이 -> 전이 Condition

	State 
		DoWork, StateName
		
	State -> State 전이는 StateMachine이 한다.

	StateMachine은 등록된 State, Goto State, Now State로 구분.

	Goto State 요청이 들어오면 현재 State가 Return이 되면 전이된다.

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

