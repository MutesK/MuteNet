
#include "Acceptor.h"
#include "Link.h"
#include "TcpSocket.h"
#include "IOService.h"
#include "LinkManager.h"
#include "IOContext.h"
namespace Network
{
	LPFN_ACCEPTEX Acceptor::AcceptEx = nullptr;

	Acceptor::Acceptor(const std::shared_ptr<IOService>& service, const std::string& ip, uint16_t port)
		:_service(service)
	{
		_bindPoint.SetConnectPoint(ip, port);
	}

	bool Acceptor::Initialize()
	{
		bool flag = true;

		_listen = std::make_shared<TcpSocket>(AF_INET);
		flag = _listen->SetReUseAddress(true);
		flag = _listen->Bind(_bindPoint);

		_service->RegisterHandle(_listen->native_handle(), 
			&_listen);

		GUID guidAcceptEx = WSAID_ACCEPTEX;
		DWORD bytes = 0;

		if (SOCKET_ERROR == WSAIoctl(_listen->socket_handle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidAcceptEx, sizeof(GUID), &AcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL))
			flag = false;

		return flag;
	}

	bool Acceptor::Start()
	{
		bool flag = true;

		flag = _listen->Listen(0);
		_acceptorThread = std::thread(std::bind(&Acceptor::PostAccept, this));

		Util::ChangeThreadName(_acceptorThread.native_handle(), "Acceptor");

		return flag;
	}

	void Acceptor::Stop()
	{
		_acceptorThread.join();
	}

	void Acceptor::PostAccept()
	{
		DWORD bytes = 0;
		DWORD flags = 0;
		byte AcceptBuf[64];

		while(true)
		{
			auto link = LinkManager::make_shared();

			const auto AcceptOverlapped = AcceptContext::OverlappedPool(link);

			if(FALSE == AcceptEx(_listen->socket_handle(), link->socket_handle(), AcceptBuf, 0,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, reinterpret_cast<LPOVERLAPPED>(AcceptOverlapped)))
			{
				if(WSAGetLastError() != WSA_IO_PENDING)
				{
					AcceptContext::OverlappedPool.Free(AcceptOverlapped);
					// Logger
				}
			}
		}
	}

	void Acceptor::AcceptCompletion(IOContext* pContext, DWORD TransferredBytes, void* CompletionKey)
	{
		if(nullptr == pContext)
		{
			// Logger
			return;
		}

		const auto AcceptIOContext = reinterpret_cast<AcceptContext *>(pContext);  // non-virtaul classes - can't use the dynamic_cast 
		auto link = AcceptIOContext->linkPtr;

		// 家南 可记 汲隆
		auto socket = link->_Socket;

		socket.SetUpdateAcceptContext(_listen->socket_handle());
		socket.SetNagle(true);
		ConnectPoint::Setter::SetConnectionPoint(socket, link->_EndPoint);

		_service->RegisterHandle(socket.native_handle(), nullptr);
		link->RecvPost();

		AcceptContext::OverlappedPool.Free(AcceptIOContext);
	}
}
