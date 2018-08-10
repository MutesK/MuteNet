#pragma once

/*
	 문자열은 호스트이름과 포트를 나타낸 스트링

	 ex)

	 192.168.10.1:18000

	 host 는 192.168.10.1이고
	 port 는 18000이다.

	 이의 정보를 SocketAddress로 만들어준다.
	
*/

class SocketAddress;
class SocketAddressFactory
{
public:
	static std::shared_ptr<SocketAddress> CreateIPv4FromString(const std::string& IN string);
};

