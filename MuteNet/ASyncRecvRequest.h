#pragma once

#include "ASyncRequest.h"
#include "LinkImpl.h"
#include "SocketUtil.h"

namespace MuteNet
{
	// Zero Byte Recv Àû¿ë ( IOCP )
	class ASyncRecvRequest : public ASyncRequest
	{
		typedef ASyncRequest super;

		static Util::TL::ObjectPool<ASyncRecvRequest, true, true> OverlappedPool;
	private:
		LinkImplPtr			  linkPtr;
		const Link::CallbacksPtr& CallbackPtr;

		friend class Util::TL::ObjectPool<ASyncRecvRequest, true, true>;

	private:
		ASyncRecvRequest(const LinkImplPtr& linkPtr)
			:linkPtr(linkPtr), CallbackPtr(linkPtr->_Callback)
		{
		}
	public:
		virtual ~ASyncRecvRequest()
		{
		}

		static ASyncRecvRequest* GetRecvRequest(const LinkImplPtr linkPtr)
		{
			return new ASyncRecvRequest(linkPtr);
		}

		static void FreeRecvRequest(ASyncRecvRequest* Ptr)
		{
			delete Ptr;
		}

		virtual bool Process() override
		{
			super::Process();

			DWORD RecvBytes = 0;
			DWORD Flag = 0;

			auto& Socket = linkPtr->_Socket;

			WSABUF buf;
			buf.buf = nullptr;
			buf.len = 0;

			if (Socket == INVALID_SOCKET)
				return false;
			
			++linkPtr->_ASyncIORequestCounter;
			int ret = WSARecv(Socket, &buf, 1, &RecvBytes, &Flag, &Overlapped.Overlapped, nullptr);

			if (ret == SOCKET_ERROR)
			{
				int errorCode = WSAGetLastError();

				if (errorCode != WSA_IO_PENDING)
				{
					CallbackPtr->OnError(errorCode, SocketUtil::ErrorString(errorCode));

					linkPtr->Shutdown();

					if (--linkPtr->_ASyncIORequestCounter <= 0)
					{
						linkPtr->Close();
					}

					return false;
				}
			}

			return true;
		}

		virtual void IOCompletion(DWORD TransfferredBytes) override
		{
			Util::CircularBuffer Buffer;

			while (true)
			{
				auto ReceivcedBytes = RecvProcess(Buffer.GetWriteBufferPtr(), Buffer.GetFreeSize());

				if (ReceivcedBytes == WSAEWOULDBLOCK)
					break;

				if (ReceivcedBytes == 0 || ReceivcedBytes == SOCKET_ERROR)
				{
					if (--linkPtr->_ASyncIORequestCounter <= 0)
					{
						linkPtr->Shutdown();
						linkPtr->Close();
					}

					FreeRecvRequest(reinterpret_cast<ASyncRecvRequest*>(Overlapped.SelfPtr));
					return;
				}

				Buffer.MoveWritePos(ReceivcedBytes);
			}

			CallbackPtr->OnReceivedData(Buffer.GetReadBufferPtr(), Buffer.GetUseSize());

			if (linkPtr->AcquireLink())
			{
				linkPtr->RecvPost();
				linkPtr->FreeLink();
			}

			if (--linkPtr->_ASyncIORequestCounter <= 0)
			{
				linkPtr->Shutdown();
				linkPtr->Close();
			}

			FreeRecvRequest(reinterpret_cast<ASyncRecvRequest *>(Overlapped.SelfPtr));
		}


		virtual void IOError(DWORD Error) override
		{
			linkPtr->Shutdown();

			if (--linkPtr->_ASyncIORequestCounter <= 0)
			{
				linkPtr->Close();
			}

			FreeRecvRequest(reinterpret_cast<ASyncRecvRequest*>(Overlapped.SelfPtr));
		}

	private:
		int RecvProcess(char* buffer, int length)
		{
			assert(length > 0);
				
			auto& Socket = linkPtr->_Socket;

			int result = ::recv(Socket, 
				reinterpret_cast<char *>(buffer), length, 0);
			if (result == SOCKET_ERROR)
			{
				auto error = WSAGetLastError();

				if (error == WSAEWOULDBLOCK)
					return WSAEWOULDBLOCK;

				return SOCKET_ERROR;
			}

			return result;
		}
	};
}