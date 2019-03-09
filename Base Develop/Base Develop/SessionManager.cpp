#include "SessionManager.h"

std::shared_ptr<Session> SessionManager::GetSession(const LinkHandle& handle)
{
	auto finditer = _SessionMap.find(handle);

	if (finditer == _SessionMap.end())
		return nullptr;

	return finditer->second;
}

std::shared_ptr<Session> SessionManager::AppendSession(const TcpSocketPtr& ptr)
{
	LinkHandle handle = reinterpret_cast<LinkHandle>(ptr->get_socket());

	auto sessionPtr = _sessionHeap.get_shared();
	_SessionMap[handle] = sessionPtr;

	return sessionPtr;
}
