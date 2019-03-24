#pragma once

#include "framework.h"
#include "Session.h"

class SessionManager : public Singleton<SessionManager>
{
public:
	std::shared_ptr<Session> GetSession(const LinkHandle& handle);
	std::shared_ptr<Session> AppendSession(const TcpSocketPtr& ptr);

	static bool Send(LinkHandle link, const std::shared_ptr<StreamBuffer> data);
	static bool Recv(LinkHandle link, size_t len);
	static bool LockReadBuffer(LinkHandle link);
	static bool UnlockReadBuffer(LinkHandle link);
private:
	CObjectPoolTLS<Session> _sessionHeap;
	std::map<LinkHandle, std::shared_ptr<Session>> _SessionMap;
};

