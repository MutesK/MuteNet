#include "StateMachine.h"



StateMachine::StateMachine()
{
	_nowState = nullptr;
	_StateThread = std::make_unique<std::thread>(std::bind(&StateMachine::DoWork, this));
}


StateMachine::~StateMachine()
{
	_StateThread->join();
}

void StateMachine::SetInitialState(const std::shared_ptr<State>& state)
{
	if (_isRunning)
		return;

	_nowState = state;
	_isRunning = true;
}

void StateMachine::RegisterState(const std::shared_ptr<State>& state)
{
	_allState[state->_statename] = state;
}

bool StateMachine::GotoState(const std::string& state)
{
	auto iter = _allState.find(state);

	if(iter == _allState.end())
		return false;

	_nowState.store(iter->second);

	return true;
}

void StateMachine::DoWork()
{
	while (!GetShutdownThreadSignal())
	{
		auto state = _nowState.load();

		if (nullptr == state)
			continue;

		state->DoWork();
	}
}
