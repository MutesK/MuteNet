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
		const GetAcceptExSockAddrsPtr& _GetAcceptExSockAddrPtr;

		SOCKET				_ClientSocket;
		char				_AddressBuffer[1024]{ 0 };
		int					_BufferLength;

		friend class Util::TL::ObjectPool<ASyncAcceptRequest, true, true>;
	private:
		ASyncAcceptRequest(const AcceptorPtr& Ptr, const AcceptExPtr& AcceptFuctPtr, const GetAcceptExSockAddrsPtr& AddrPtr)
			:_Acceptor(Ptr), _AcceptExFunctionPtr(AcceptFuctPtr), _BufferLength(1024), _GetAcceptExSockAddrPtr(AddrPtr)
		{
		}
	public:
		virtual ~ASyncAcceptRequest() = default;

		static ASyncAcceptRequest* GetAcceptRequest(const AcceptorPtr& Ptr, const AcceptExPtr& AcceptFuctPtr, const GetAcceptExSockAddrsPtr& AddrPtr)
		{
			return new ASyncAcceptRequest(Ptr, AcceptFuctPtr, AddrPtr);
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

			if(!_AcceptExFunctionPtr(ListenSocket, _ClientSocket, _AddressBuffer, 0,
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
				closesocket(_ClientSocket);
				const auto error = WSAGetLastError();

				_Acceptor->_ErrorCallback(_ClientSocket, error, SocketUtil::ErrorString(error));
				_Acceptor->StartAccept();
			}

			SOCKADDR* RemoteSockAddr = nullptr, *LocalSockAddr = nullptr;
			int RemoteAddrLength = 0, LocalAddrLength = 0;

			_GetAcceptExSockAddrPtr(_AddressBuffer, TransfferredBytes, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
				&LocalSockAddr, &RemoteAddrLength, &RemoteSockAddr, &RemoteAddrLength);

			_Acceptor->_Callback(_ClientSocket, RemoteSockAddr, RemoteAddrLength, _Acceptor->_Key);
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
