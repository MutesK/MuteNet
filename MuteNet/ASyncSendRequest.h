#pragma once

#include "ASyncRequest.h"
#include "LinkImpl.h"
#include "SocketUtil.h"

namespace MuteNet
{
	class ASyncSendRequest : public ASyncRequest
	{
		static Util::TL::ObjectPool<ASyncSendRequest, true, true> OverlappedPool;

	private:
		LinkImplPtr			linkPtr;
		char*			inData = nullptr;
		size_t			length = 0;
		const Link::CallbacksPtr& CallbackPtr;

		friend class Util::TL::ObjectPool<ASyncSendRequest, true, true>;
	private:
		ASyncSendRequest(const LinkImplPtr linkPtr, char* inbuffer, size_t length)
			:linkPtr(linkPtr), CallbackPtr(linkPtr->_Callback), inData(inbuffer), length(length)
		{
		}
	public:
		virtual ~ASyncSendRequest() = default;

		static ASyncSendRequest* GetSendRequest(const LinkImplPtr& linkPtr, char* inbuffer, size_t length)
		{
			return new ASyncSendRequest(linkPtr, inbuffer, length);
		}

		static void FreeSendRequest(ASyncSendRequest* Ptr)
		{
			delete Ptr;
		}

		virtual bool Process() override
		{
			__super::Process();

			++linkPtr->_ASyncIORequestCounter;

			WSABUF Buf;
			Buf.buf = const_cast<char *>(inData);
			Buf.len = length;

			auto& Socket = linkPtr->_Socket;

			if (Socket == INVALID_SOCKET)
				return false;

			DWORD SendBytes = 0;
			auto ret = ::WSASend(Socket, &Buf, 1, &SendBytes, 0, &Overlapped.Overlapped, nullptr);

			if (ret == SOCKET_ERROR)
			{
				auto errorCode = WSAGetLastError();

				if (errorCode != WSA_IO_PENDING)
				{
					CallbackPtr->OnError(errorCode, SocketUtil::ErrorString(errorCode));

					if (--linkPtr->_ASyncIORequestCounter <= 0)
					{
						linkPtr->Shutdown();
						linkPtr->Close();
					}

					return false;
				}
			}

			return true;
		}

		virtual void IOCompletion(DWORD TransfferredBytes) override
		{
			if (--linkPtr->_ASyncIORequestCounter <= 0)
			{
				linkPtr->Shutdown();
				linkPtr->Close();
			}

			FreeSendRequest(reinterpret_cast<ASyncSendRequest*>(Overlapped.SelfPtr));

		}

		virtual void IOError(DWORD Error) override
		{
			CallbackPtr->OnError(Error, SocketUtil::ErrorString(Error));

			if (--linkPtr->_ASyncIORequestCounter <= 0)
			{
				linkPtr->Shutdown();
				linkPtr->Close();
			}

			FreeSendRequest(reinterpret_cast<ASyncSendRequest*>(Overlapped.SelfPtr));
		}
	};
}