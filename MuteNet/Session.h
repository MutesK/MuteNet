#pragma once

#include "pch.h"

namespace MuteNet
{
	class SessionInfo;
	typedef void (*RecvCallback)(SessionInfo*, void* Key);
	typedef void (*SendCallback)(SessionInfo*, void* Key);

	struct Overlapped
	{
		OVERLAPPED		_OverlappedIO;
		void (*Callback)(DWORD, void*);
	};

	class ServiceListener;
	class SessionInfo final
	{
	private:
		SOCKET						_Socket;
		ServiceListener*			_EventBase;

		Overlapped					_Send;
		Overlapped					_Recv;

		Util::CircularBuffer		_SendBuffer;
		Util::CircularBuffer		_RecvBuffer;

		RecvCallback				_RecvCb;
		SendCallback				_SendCb;
		void*						_CompletionKey;

		friend class ServiceListener;
	public:
		static SessionInfo* NewSessionInfo(ServiceListener* Listener, intptr_t Fd);
		static void Free(SessionInfo* Info);

		void Enable();
		void Disable();
		void Shutdown();
		void SetCallback(RecvCallback Recv, SendCallback Send, void* Key);

		SOCKET GetFD();

		void Send(const void* Data, size_t Length);
		int Connect(const sockaddr* Ip, int Size);
		
	private:
		SessionInfo(ServiceListener* Listener, SOCKET Fd);

		// 차후 Zero-Byte Copy 생각해본다.
		void RecvPost();
		static void OnSend(DWORD TransfferredBytes, void* SessionInfo);
		static void OnRecv(DWORD TransfferredBytes, void* SessionInfo);

		int RecvProcess(char* bufferPos, int length);
	};

	inline SOCKET SessionInfo::GetFD()
	{
		return _Socket;
	}

	inline void SessionInfo::Free(SessionInfo* Info)
	{
		delete Info;
	}
}

