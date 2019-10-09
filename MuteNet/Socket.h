#pragma once

namespace MuteNet
{
	class ConnectPoint;

	enum class ShutdownBlockMode
	{
		RecvBlock = SD_RECEIVE,
		SendBlock = SD_SEND,
		BothBlock = SD_BOTH,
	};

	class Socket
	{
	protected:
		Socket(ADDRESS_FAMILY f = AF_INET);
	public:
		virtual ~Socket();

		int SetIoMode(bool ioflag);

		int GetLastError() const;

		int GetAddress(ConnectPoint& OUT Point) const;

		int SetRecvSocketBufferSize(int size) const;
		int SetSendSocketBufferSize(int size) const;

		int SetReUseAddress(bool toggle) const;

		int SetRecvCallLimitTime(DWORD time) const;
		int SetSendCallLimitTime(DWORD time) const;

		int SetUseKeepAlive(bool toggle) const;

		int Shutdown(ShutdownBlockMode Mode);

		SOCKET socket_handle() const;
		void set_socket_handle(SOCKET handle);

		HANDLE native_handle() const;
		void set_native_handle(HANDLE handle);

		friend class Link;
	protected:
		SOCKET			_handle;
		DWORD			_lastError;
		ADDRESS_FAMILY	_address_family;
	};

	inline int Socket::GetLastError() const
	{
		return _lastError;
	}

	inline int Socket::SetRecvSocketBufferSize(int size) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char*>(&size), sizeof(int));
	}

	inline int Socket::SetSendSocketBufferSize(int size) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char*>(&size), sizeof(int));
	}

	inline int Socket::SetReUseAddress(bool toggle) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&toggle), sizeof(bool));
	}

	inline int Socket::SetRecvCallLimitTime(DWORD time) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&time), sizeof(DWORD));
	}

	inline int Socket::SetSendCallLimitTime(DWORD time) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&time), sizeof(DWORD));
	}

	inline int Socket::SetUseKeepAlive(bool toggle) const
	{
		return setsockopt(_handle, SOL_SOCKET, SO_KEEPALIVE, reinterpret_cast<const char*>(&toggle), sizeof(bool));
	}

	inline SOCKET Socket::socket_handle() const
	{
		return _handle;
	}

	inline void Socket::set_socket_handle(SOCKET handle)
	{
		_handle = handle;
	}

	inline HANDLE Socket::native_handle() const
	{
		return reinterpret_cast<HANDLE>(_handle);
	}
	inline void Socket::set_native_handle(HANDLE handle)
	{
		_handle = reinterpret_cast<SOCKET>(handle);
	}

}
