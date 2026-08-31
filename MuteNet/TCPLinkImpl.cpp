#include "TCPLinkImpl.h"
#include "Socket.h"
#include "IODispatcher.h"
#include "ASyncSendRequest.h"
#include "ASyncRecvRequest.h"
#include "Allocator.h"
#include <utility>

namespace MuteNet
{
	TCPLinkImpl::TCPLinkImpl(CallbacksPtr callbacks, 
		std::shared_ptr<IODispatcher> dispatcher)
		: ALink(std::move(callbacks)),
		ioDispatcher(std::move(dispatcher))
	{
		auto rawSock = Allocator::New<Socket>();
		socket = std::shared_ptr<Socket>(rawSock,
			[](Socket* ptr) {
				Allocator::Delete(ptr);
			});

		socket->Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	TCPLinkImpl::TCPLinkImpl(std::shared_ptr<Socket> socket, 
		CallbacksPtr callbacks, std::shared_ptr<IODispatcher> dispatcher)
		: ALink(std::move(callbacks)),
		socket(std::move(socket)),
		ioDispatcher(std::move(dispatcher))
	{
	}

	TCPLinkImpl::~TCPLinkImpl()
	{
		Close();
	}

	void TCPLinkImpl::SetDispatcher(std::shared_ptr<IODispatcher> dispatcher)
	{
		ioDispatcher = std::move(dispatcher);
	}

	void TCPLinkImpl::Enable()
	{
		if (!socket || socket->IsValid())
		{
			return;
		}

		if (ioDispatcher)
		{
			ioDispatcher->RegisterSocket(socket->GetHandle(), this);
		}
		else
		{
			throw std::runtime_error("IODispatcher is not set.");
		}

		++aSyncIORequestCounter;
		RecvPost();

		if (--aSyncIORequestCounter == 0)
		{
			Close();
		}
	}

	bool TCPLinkImpl::Send(const char* data, std::size_t length)
	{
		if (isShutdown || !socket || !socket->IsValid())
		{
			return false;
		}

		auto Request =
			ASyncSendRequest::GetSendRequest(shared_from_this(), GetSocket(), 
				const_cast<char*>(data), length, GetCallbacks());
		if (!Request->Process())
		{
			ASyncSendRequest::FreeSendRequest(Request);
			return false;
		}

		return true;
	}

	void TCPLinkImpl::Shutdown()
	{
		//socket->s
	}

	void TCPLinkImpl::Close()
	{
		isShutdown = true;

		GetCallbacks()->OnRemoteClosed(this);
		socket->Close();
	}

	void TCPLinkImpl::RecvPost()
	{
		if (isShutdown || !socket || !socket->IsValid())
		{
			return;
		}

		auto Request = ASyncRecvRequest::GetRecvRequest(shared_from_this(), GetSocket(), 4096);
		if (!Request->Process())
		{
			ASyncRecvRequest::FreeRecvRequest(Request);
			Close();
		}
	}






}