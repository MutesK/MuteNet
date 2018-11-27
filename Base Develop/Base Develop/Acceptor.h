#pragma once

#include "Thread.h"
#include <functional>

/*
	
*/
class CSession;
class Acceptor : public Thread
{
public:
	Acceptor(const std::string& IpAddress);
	virtual ~Acceptor();

protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;
	

	virtual void OnAccept(std::shared_ptr<CSession> Session) = 0;
private:

	std::string  _ipAddress;
	bool		_socketMode;
	bool		_threadstoprequest;
	bool		_isUseNagle;
};

