#pragma once

#include "ASyncRequest.h"
#include "SocketUtil.h"
#include "Acceptor.h"
#include "SocketIO.h"
#include "NetworkManager.h"

namespace MuteNet
{
	class ASyncAcceptRequest : public ASyncRequest
	{
		typedef ASyncRequest super;

		static Util::TL::ObjectPool<ASyncAcceptRequest, true, true> OverlappedPool;
	private:
		const AcceptorPtr& _Acceptor;
		const AcceptExPtr& _AcceptExFunctionPtr;

		SOCKET				_ClientSocket;
		char				_AddressBuffer[1024]{ 0 };
		int					_BufferLength;

		friend class Util::TL::ObjectPool<ASyncAcceptRequest, true, true>;
	private:
		ASyncAcceptRequest(const AcceptorPtr& Ptr, const AcceptExPtr& AcceptFuctPtr)
			:_Acceptor(Ptr), _AcceptExFunctionPtr(AcceptFuctPtr), _BufferLength(1024)
		{
		}
	public:
		virtual ~ASyncAcceptRequest() = default;

		static ASyncAcceptRequest* GetAcceptRequest(const AcceptorPtr& Ptr, const AcceptExPtr& AcceptFuctPtr)
		{
			return new ASyncAcceptRequest(Ptr, AcceptFuctPtr);
		}

		static void FreeAcceptRequest(ASyncAcceptRequest* Ptr)
		{
			delete Ptr;
		}

		virtual bool Process() override
		{
			super::Process();

			auto ListenSocket = _Acceptor->_Listen;

			_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (_ClientSocket == INVALID_SOCKET)
			{
				const auto error = WSAGetLastError();

				_Acceptor->_ErrorCallback(_ClientSocket, error, SocketUtil::ErrorString(error));
				return false;
			}

			DWORD Pending = 0;

			if(!_AcceptExFunctionPtr(ListenSocket, _ClientSocket, _AddressBuffer, _BufferLength - ((sizeof(SOCKADDR_IN) + 16) * 2),
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Pending, &Overlapped.Overlapped))
			{
				const auto error = WSAGetLastError();

				if (error == WSA_IO_PENDING)
					return true;

				closesocket(_ClientSocket);
				_Acceptor->_ErrorCallback(_ClientSocket, error, SocketUtil::ErrorString(error));
				return false;
			}

			return true;
		}

		virtual void IOCompletion(DWORD TransfferredBytes) override
		{
			auto ListenSocket = _Acceptor->_Listen;

			if (setsockopt(_ClientSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
				reinterpret_cast<char*>(&ListenSocket), sizeof(ListenSocket)) == SOCKET_ERROR)
			{
				const auto error = WSAGetLastError();

				_Acceptor->_ErrorCallback(_ClientSocket, error, SocketUtil::ErrorString(error));
				_Acceptor->StartAccept();
			}

			SOCKADDR_IN RemoteSockAddr;
			int RemoteSocketLength = sizeof(struct sockaddr_in);

			getpeername(_ClientSocket, reinterpret_cast<SOCKADDR*>(&RemoteSockAddr), &RemoteSocketLength);

			_Acceptor->_Callback(_ClientSocket, reinterpret_cast<SOCKADDR *>(&RemoteSockAddr), RemoteSocketLength, _Acceptor->_Key);
			_Acceptor->StartAccept();

			FreeAcceptRequest(reinterpret_cast<ASyncAcceptRequest *>(Overlapped.SelfPtr));
		}

		virtual void IOError(DWORD Error) override
		{
			_Acceptor->_ErrorCallback(_ClientSocket, Error, SocketUtil::ErrorString(Error));
			_Acceptor->StartAccept();

			FreeAcceptRequest(reinterpret_cast<ASyncAcceptRequest*>(Overlapped.SelfPtr));
		}

	};
}
