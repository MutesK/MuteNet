#include "Acceptor.h"
#include "Network.h"
#include "SessionManager.h"


Acceptor::Acceptor(const std::shared_ptr<IOCPManager>& SocketIO, const std::string& DnsAddress)
	: _SocketIO(SocketIO)
{
	auto address = SocketAddressFactory::CreateSocketAddress(DnsAddress);

	_listensocket = SocketUtil::CreateTCPSocket(AF_INET);
	_listensocket->bind(address);
	_listensocket->listen(0);

	if (_listensocket->getLastError() == SOCKET_ERROR)
		throw std::exception("Listen Socket Bind Error");
}

Acceptor::~Acceptor()
{
	Destory();
}

void Acceptor::Start()
{
}

void Acceptor::Stop()
{
}

void Acceptor::Destory()
{
}

void Acceptor::DoWork()
{
	while (true)
	{
		if (Network::_currentSession >= Network::_maxSession)
			continue;

		TcpSocketPtr socket = _listensocket->Accept();

		if (socket->get_handle() == INVALID_SOCKET)
			continue;

		auto session = SessionManager::GetInstance()->AppendSession(socket);
		_SocketIO->RegisterSocket(session);
		session->RecvPost();

		Network::OnAccept(session->getHandle());
			
	}
}