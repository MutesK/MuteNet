// SelectServerTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#include "pch.h"
#include <random>
#include <thread>
#include <chrono>
#include <vector>
#include "../Base Develop/RingBuffer.h"
#include "../Base Develop/Select.h"
#include "../Base Develop/SocketUtil.h"

using namespace std;


#define SIZE 1024

void OnRecv(TcpSocketPtr socket);
void OnAccept(TcpSocketPtr socket);
void OnSend(TcpSocketPtr socket);
void OnExcept(TcpSocketPtr socket);

std::vector<TcpSocketPtr> _clientSocketList;
std::shared_ptr<SelectIO> IOService;
int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	auto listen = SocketUtil::CreateTCPSocket(AF_INET);
	auto address = SocketAddress(INADDR_ANY, 50000);

	listen->bind(address);
	listen->listen(0);
	IOService = std::make_shared<SelectIO>(listen, OnAccept, OnRecv, OnSend, OnExcept);

	IOService->Start();

	while (1)
	{

	}

	IOService->Stop();
}

void OnRecv(TcpSocketPtr socket)
{
	char bytes[SIZE];

	int dataRecv = socket->Recv(bytes, 1024);
	std::cout << "Data Recived\n";

	if (dataRecv == 0)
	{
		std::cout << "Client Disconnect" << std::endl;

		IOService->DequeueSocket(socket);
	}
	else
	{
		// Echo
		socket->Send((void *)bytes, dataRecv);
	}



}
void OnAccept(TcpSocketPtr socket)
{
	std::cout << "Client Connected" << std::endl;

	IOService->EnqueueSocket(socket);
}
void OnSend(TcpSocketPtr socket)
{
	std::cout << "Data Sended\n";
}

void OnExcept(TcpSocketPtr socket)
{
	std::cout << "Excepted\n";

	IOService->DequeueSocket(socket);
}
