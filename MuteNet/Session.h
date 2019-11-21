#pragma once

#include "pch.h"

namespace MuteNet
{

	struct Overlapped
	{
		OVERLAPPED		_OverlappedIO;
		void (*Callback)(DWORD, void*);
	};

	class ServiceListener;
	class SessionInfo
	{
	private:
		SOCKET						_Socket;
		ServiceListener*			_EventBase;

		Overlapped					_Send;
		Overlapped					_Recv;

		Util::CircularBuffer		_SendBuffer;
		Util::CircularBuffer		_RecvBuffer;

		void*						_CompletionKey;

		friend class ServiceListener;
	public:
		void Enable();
		void Disable();
		void Shutdown();

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


}

