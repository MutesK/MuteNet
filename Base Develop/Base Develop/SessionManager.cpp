#include "SessionManager.h"

std::shared_ptr<Session> SessionManager::GetSession(const LinkHandle& handle)
{
	auto session = SessionManager::GetInstance();
	auto finditer = _SessionMap.find(handle);

	if (finditer == _SessionMap.end())
		return nullptr;

	return finditer->second;
}

std::shared_ptr<Session> SessionManager::AppendSession(const TcpSocketPtr& ptr)
{
	LinkHandle handle = reinterpret_cast<LinkHandle>(ptr->get_handle());

	auto sessionPtr = _sessionHeap.get_shared();
	sessionPtr->_socket = ptr;
	_SessionMap[handle] = sessionPtr;

	return sessionPtr;
}

void SessionManager::RemoveSession(const LinkHandle& handle)
{
	_SessionMap.erase(handle);
}
