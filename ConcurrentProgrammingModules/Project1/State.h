#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"

class State abstract
{
public:
	State(const std::string& statename);
	~State();

	virtual void DoWork() = 0;
private:
	friend class StateMachine;

	std::string _statename;
};

