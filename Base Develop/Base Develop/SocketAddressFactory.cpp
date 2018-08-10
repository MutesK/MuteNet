#include "stdafx.h"
#include "SocketAddressFactory.h"
#include "SocketAddress.h"

using namespace std;

std::shared_ptr<SocketAddress> SocketAddressFactory::CreateIPv4FromString(const std::string& IN str)
{
	auto pos = str.find_last_of(':');

	string host, service;
	if (pos != string::npos)  // 찾는 문자열이 있다면
	{
		host = str.substr(0, pos);
		service = str.substr(pos + 1);
	}
	else
	{
		host = str;
		service = "0";
	}

	// 네트워크 정보 가져옴.
	addrinfo hint;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	
	addrinfo* result = nullptr;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
	addrinfo* initresult = nullptr;

	if (error != 0 && result != nullptr)
	{
		freeaddrinfo(initresult);
		return nullptr;
	}

	while (!result->ai_addr && result->ai_next)
		result = result->ai_next;

	if (!result->ai_addr)
	{
		freeaddrinfo(initresult);
		return nullptr;
	}

	auto ret = std::make_shared<SocketAddress>(*(result->ai_addr));
	freeaddrinfo(initresult);

	return ret;

}