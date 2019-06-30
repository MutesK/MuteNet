
#include "Acceptor.h"
#include "Link.h"
#include "IOEngine.h"
#include "TcpSocket.h"
#include "IOService.h"
#include "IOContext.h"
#include "AddReadIO.h"


namespace Network
{
	Acceptor::Acceptor(const std::shared_ptr<IOService>& service,
		const std::string& ip, uint16_t port)
		:_Service(service)
	{
		_Listen = std::make_unique<TcpSocket>(AF_INET);
		_Service->RegisterHandle(_Listen->native_handle(), this);

		_Listen->Bind(ip, port);
		_Listen->Listen(0);
		

		AcceptPost();
	}

	void Acceptor::ProcessAccept(AcceptIOContext& Context)
	{
		// Ŭ���̾�Ʈ ���� (Link, TcpSocket, EndPoint) ����
		// �ش� ���� RecvPost

		// Ŭ���̾�Ʈ Ǯ�� �ʿ伺!!!!
		Link* pLink = new Link(Context._Socket);

		SOCKADDR* LocalSockAddr = nullptr;
		int LocalSize = 0;

		SOCKADDR* RemoteSockAddr = nullptr;
		int RemoteSize = 0;

		GetAcceptExSockaddrs(Context._buffer, AddressLength * 2, AddressLength, AddressLength, &LocalSockAddr, &LocalSize,
			&RemoteSockAddr, &RemoteSize);

		pLink->SetEndPoint(LocalSockAddr);
		pLink->SetRemotePoint(RemoteSockAddr);


		AcceptPost();
	}


	void Acceptor::AcceptPost()
	{
		memset(&_Context, 0, sizeof(AcceptIOContext));

		_Context._Type = OverlappedType::eAccepted;
		_Context._Socket = new TcpSocket(AF_INET);  // ����Ǯ �ʿ伺. !!

		DWORD Recvied = 0;

		const auto result = _AcceptEx(reinterpret_cast<SOCKET>(_Listen->native_handle()),
			reinterpret_cast<SOCKET>(_Context._Socket->native_handle()),
			_Context._buffer, AddressLength * 2,
			AddressLength, AddressLength, &Recvied,
			reinterpret_cast<LPOVERLAPPED>(&_Context));


		if(result == SOCKET_ERROR)
		{
			const auto error = WSAGetLastError();

			if(error != WSA_IO_PENDING)
			{
				throw;
			}
		}
	}

	void Acceptor::LoadAcceptExFP()
	{
		DWORD bytes;

		WSAIoctl(reinterpret_cast<SOCKET>(_Listen->native_handle()), SIO_GET_EXTENSION_FUNCTION_POINTER,
				&_GuidAcceptEx, sizeof(_GuidAcceptEx),
				&_AcceptEx, sizeof(_AcceptEx),
				&bytes, nullptr, nullptr);
	}

}
