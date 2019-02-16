// SelectClientTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "../Base Develop/SocketAddressFactory.h"
#include "../Base Develop/Connector.h"
#include "../Base Develop/Select.h"
#include <iostream>

#define SIZE 1024


void OnAccept(TcpSocketPtr ptr){}  // Not Use
void OnRecv(TcpSocketPtr ptr) 
{
	char bytes[SIZE];

	int dataRecv = ptr->Recv(bytes, 1024);
	std::cout << "Data Recived\n";

	if (dataRecv == 0)
	{
		std::cout << "Client Disconnect" << std::endl;
		throw;
	}
	else
	{
		// Echo
		ptr->Send((void *)bytes, dataRecv);
	}

}
void OnSend(TcpSocketPtr ptr) 
{
	std::cout << "Data Sended\n";
}
void OnExcpet(TcpSocketPtr ptr) 
{
}

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	auto Address = SocketAddressFactory::CreateSocketAddress("127.0.0.1:50000");

	auto ClientSock = Connector::GetClientTcpSocket(AF_INET, *Address);

	if (ClientSock->get_socket() == INVALID_SOCKET)
	{
		std::cout << ClientSock->getLastError() << std::endl;
		return 0;
	}

	std::cout << "Server Connected \n";

	// Echo 
	SelectIO IOService(ClientSock, OnRecv, OnSend, OnExcpet);
	IOService.Start();

	char buffer[] = "sdlfjsl dfksjdflksdjflksdjfl sdkjf lsdk jfsldkf jsldkf jsdlkfj asl;kfja s;ldkfj asl;dkfjsa ldkfj aslkdfj aslkdjf ls;akjf la;skdjf salkdf";
	ClientSock->Send(buffer, strlen(buffer));

	while (1)
	{

	}

}

