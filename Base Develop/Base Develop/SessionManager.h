#pragma once

#include "framework.h"
#include "Session.h"
#include "StreamBuffer.h"

class SessionManager : public Singleton<SessionManager>
{
public:
	std::shared_ptr<Session> GetSession(const LinkHandle& handle);
	std::shared_ptr<Session> AppendSession(const TcpSocketPtr& ptr);
	void RemoveSession(const LinkHandle& handle);

	static bool Send(LinkHandle link, const std::shared_ptr<StreamBuffer> data);
	static bool Recv(LinkHandle link, size_t len);
private:
	CObjectPoolTLS<Session> _sessionHeap;

	std::map<LinkHandle, std::shared_ptr<Session>> _SessionMap;
};

