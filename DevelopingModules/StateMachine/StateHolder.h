#pragma once
/*
	���� State�� ��Ÿ����.

	���� State�� Work�� ó���Ѵ�.

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

