#pragma once

#include "ASyncRequest.h"
#include "LinkImpl.h"
#include "SocketUtil.h"

namespace MuteNet
{
	class ASyncSendRequest : public ASyncRequest
	{
		typedef ASyncRequest super;

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
			return OverlappedPool(linkPtr, inbuffer, length);
		}

		static void FreeSendRequest(ASyncSendRequest* Ptr)
		{
			OverlappedPool.Free(Ptr);
		}

		virtual bool Process() override
		{
			super::Process();

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

					if (--linkPtr->_ASyncIORequestCounter == 0)
					{
						linkPtr->Close();
					}

					linkPtr->Shutdown();

					return false;
				}
			}

			return true;
		}

		virtual void IOCompletion(DWORD TransfferredBytes) override
		{
			if (--linkPtr->_ASyncIORequestCounter == 0)
			{
				linkPtr->Close();
			}

			FreeSendRequest(reinterpret_cast<ASyncSendRequest*>(Overlapped.SelfPtr));

		}

		virtual void IOError(DWORD Error) override
		{
			CallbackPtr->OnError(Error, SocketUtil::ErrorString(Error));

			if (--linkPtr->_ASyncIORequestCounter == 0)
			{
				linkPtr->Close();
			}

			FreeSendRequest(reinterpret_cast<ASyncSendRequest*>(Overlapped.SelfPtr));
		}
	};
}