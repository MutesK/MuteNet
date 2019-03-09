#pragma once

#include "framework.h"
#include "Session.h"

class SessionManager : public Singleton<SessionManager>
{
public:
	std::shared_ptr<Session> GetSession(const LinkHandle& handle);
	std::shared_ptr<Session> AppendSession(const TcpSocketPtr& ptr);
private:
	CObjectPoolTLS<Session> _sessionHeap;
	std::map<LinkHandle, std::shared_ptr<Session>> _SessionMap;
};

