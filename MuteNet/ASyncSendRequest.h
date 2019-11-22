#pragma once

#include "ASyncRequest.h"
#include "LinkImpl.h"
#include "SocketUtil.h"

namespace MuteNet
{
	class ASyncSendRequest : public ASyncRequest
	{
	private:
		LinkImplPtr			linkPtr;
		const char*			inData;
		const size_t		length;
		const Link::CallbacksPtr& CallbackPtr;
	public:
		ASyncSendRequest(const LinkImplPtr& linkPtr, const char* Data, size_t Length)
			:linkPtr(linkPtr), inData(Data), length(Length), CallbackPtr(linkPtr->_Callback)
		{
		}

		virtual bool Process() override
		{
			WSABUF Buf;
			Buf.buf = const_cast<char *>(inData);
			Buf.len = length;

			auto& Socket = linkPtr->_Socket;

			DWORD SendBytes = 0;
			auto ret = ::WSASend(Socket, &Buf, 1, &SendBytes, 0, this, nullptr);

			if (ret == SOCKET_ERROR)
			{
				auto errorCode = WSAGetLastError();

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
			if (--linkPtr->_ASyncIORequestCounter == 0)
			{
				linkPtr->Close();
			}

			delete this;
		}
	};
}