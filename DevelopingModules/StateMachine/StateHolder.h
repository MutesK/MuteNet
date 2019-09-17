#pragma once
/*
	현재 State를 나타낸다.

	현재 State의 Work를 처리한다.

*/
class State;
class StateHolder final
{
public:
	StateHolder();
	~StateHolder();

private:
	std::shared_ptr<State> _state = nullptr;
};

