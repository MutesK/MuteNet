#include "Common.h"
#include "UDPLinkImpl.h"
#include "Socket.h"
#include "IODispatcher.h"
#include "ASyncSendRequest.h"
#include "ASyncRecvRequest.h"
#include "Allocator.h"
#include <utility>
#include <stdexcept>

namespace MuteNet
{
	UDPLinkImpl::UDPLinkImpl(CallbacksPtr callbacks, std::shared_ptr<IODispatcher> dispatcher)
		: ALink(std::move(callbacks)), ioDispatcher(std::move(dispatcher))
	{
		auto rawSock = Allocator::New<Socket>();
		socket = std::shared_ptr<Socket>(rawSock, [](Socket* ptr) {
			Allocator::Delete(ptr);
			});

		socket->Create(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}

	UDPLinkImpl::UDPLinkImpl(std::shared_ptr<Socket> socket, CallbacksPtr callbacks, std::shared_ptr<IODispatcher> dispatcher)
		: ALink(std::move(callbacks)), socket(std::move(socket)), ioDispatcher(std::move(dispatcher))
	{
	}

	UDPLinkImpl::~UDPLinkImpl()
	{
		Close();
	}

	void UDPLinkImpl::SetDispatcher(std::shared_ptr<IODispatcher> dispatcher)
	{
		ioDispatcher = std::move(dispatcher);
	}

	void UDPLinkImpl::Enable()
	{
		if (!socket || !socket->IsValid())
		{
			return;
		}

		if (ioDispatcher)
		{
			ioDispatcher->RegisterSocket(socket->GetHandle(), this);
		}
		else
		{
			throw std::runtime_error("IODispatcher is not set for UDPLinkImpl.");
		}

		RecvPost();
	}

	bool UDPLinkImpl::Send(const char* data, std::size_t length)
	{
		// UDP는 기본적으로 연결형이 아닐 수 있으므로 기본 Send 구현 또는 SendTo 연동
		return SendTo(data, length, remoteIP, remotePort);
	}

	bool UDPLinkImpl::SendTo(const char* data, std::size_t length, const std::string& ip, uint16_t port)
	{
		if (isShutdown || !socket || !socket->IsValid())
		{
			return false;
		}

		auto Request = ASyncSendRequest::GetSendRequest(shared_from_this(), GetSocket(),
			const_cast<char*>(data), length, GetCallbacks());

		if (!Request->Process())
		{
			ASyncSendRequest::FreeSendRequest(Request);
			return false;
		}

		return true;
	}

	void UDPLinkImpl::Shutdown()
	{
		// UDP 비동기 종료 처리
	}

	void UDPLinkImpl::Close()
	{
		if (isShutdown) return;
		isShutdown = true;

		if (callbacks)
		{
			callbacks->OnRemoteClosed(this);
		}

		if (socket)
		{
			socket->Close();
		}
	}

	void UDPLinkImpl::RecvPost()
	{
		if (isShutdown || !socket || !socket->IsValid())
		{
			return;
		}

		auto Request = ASyncRecvRequest::GetRecvRequest(shared_from_this(), GetSocket(), 65535); 
		if (!Request->Process())
		{
			ASyncRecvRequest::FreeRecvRequest(Request);
			Close();
		}
	}
}