#pragma once

/*
	sockaddr�� ���� Type Safe�ϰ�, 
	Ư�� ���� API�� �������� �ʵ��� �����Ѵ�.
*/
class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& IN sockaddr);
	size_t GetSize() const;
	~SocketAddress();

	GET_SET_ATTRIBUTE(sockaddr, addr);
	GET_CONST_ATTRIBUTE(sockaddr, addr);
private:
	sockaddr_in* GetSockAddrIn();
private:
	sockaddr  _addr;
};

