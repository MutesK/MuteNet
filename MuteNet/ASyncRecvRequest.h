#pragma once

#include "ASyncRequest.h"
#include "LinkImpl.h"
#include "SocketUtil.h"

namespace MuteNet
{
	// Zero Byte Recv Àû¿ë ( IOCP )
	class ASyncRecvRequest : public ASyncRequest
	{
	private:
		LinkImplPtr			  linkPtr;
		const Link::CallbacksPtr& CallbackPtr;
	public:
		ASyncRecvRequest(const LinkImplPtr& linkPtr)
			:linkPtr(linkPtr), CallbackPtr(linkPtr->_Callback)
		{
		}

		virtual bool Process() override
		{
			DWORD RecvBytes = 0;

			auto& Socket = linkPtr->_Socket;

			int ret = WSARecv(Socket, nullptr, 0, &RecvBytes, nullptr, this, nullptr);

			if (ret == SOCKET_ERROR)
			{
				int errorCode = WSAGetLastError();

				if (errorCode != WSA_IO_PENDING)
				{
					CallbackPtr->OnError(errorCode, SocketUtil::ErrorString(errorCode));
					return false;
				}
			}

			++linkPtr->_ASyncIORequestCounter;
			return true;
		}

		virtual void IOCompletion(DWORD TransfferredBytes) override
		{
			static constexpr int OnceRecvBytes = 5000;

			Byte* TempRecvBuffer = new Byte[OnceRecvBytes];
			auto ReceivcedBytes = RecvProcess(TempRecvBuffer, OnceRecvBytes);

			if(ReceivcedBytes == SOCKET_ERROR)
			{
				delete this;
				return;
			}

			CallbackPtr->OnReceivedData(reinterpret_cast<char *>(TempRecvBuffer), ReceivcedBytes);
			delete[] TempRecvBuffer;

			if (--linkPtr->_ASyncIORequestCounter == 0)
			{
				linkPtr->Close();
			}

			delete this;
		}

	private:
		int RecvProcess(Byte* buffer, int length)
		{
			assert(length > 0);
				
			auto& Socket = linkPtr->_Socket;

			int result = ::recv(Socket, 
				reinterpret_cast<char *>(buffer), length, 0);
			if (result == SOCKET_ERROR)
			{
				CallbackPtr->OnError(result, SocketUtil::ErrorString(result));
				return SOCKET_ERROR;
			}

			return result;
		}
	};
}