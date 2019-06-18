#pragma once

#include "MuteNetFoundation.h"

namespace Network
{
	class EndPoint final
	{
	public:
		EndPoint(const std::string& address, uint16_t inPort);
		EndPoint(const EndPoint& address);
		EndPoint(const sockaddr& sockAddr);
		EndPoint(const sockaddr_in& sockAddr_in);
		EndPoint(EndPoint&& Point);
		~EndPoint() = default;

		EndPoint& operator=(EndPoint&& Point) noexcept;

		const sockaddr* GetSocketAddress();
		std::string GetIpAddress() const;
		int	GetPort() const;

		static int GetSize();
	private:
		void CreateSockAddr(const std::string& address, uint16_t port);
	private:
		sockaddr_in _sockAddr{};
	};


	inline EndPoint& EndPoint::operator=(EndPoint&& Point) noexcept
	{
		_sockAddr = std::move(Point._sockAddr);
		return *this;
	}

	inline const sockaddr* EndPoint::GetSocketAddress()
	{
		return reinterpret_cast<const sockaddr*>(&_sockAddr);
	}

	inline int EndPoint::GetPort() const
	{
		return ntohs(_sockAddr.sin_port);
	}

	inline int EndPoint::GetSize()
	{
		return sizeof(sockaddr);
	}

}
