#pragma once

/*
	 ���ڿ��� ȣ��Ʈ�̸��� ��Ʈ�� ��Ÿ�� ��Ʈ��

	 ex)

	 192.168.10.1:18000

	 host �� 192.168.10.1�̰�
	 port �� 18000�̴�.

	 ���� ������ SocketAddress�� ������ش�.
	
*/

class SocketAddress;
class SocketAddressFactory
{
public:
	static std::shared_ptr<SocketAddress> CreateIPv4FromString(const std::string& IN string);
};

