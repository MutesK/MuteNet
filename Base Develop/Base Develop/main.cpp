#include "RingBuffer.h"
#include <random>
#include <thread>
#include <chrono>
#include "Select.h"
#include "SocketUtil.h"
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
	auto listen = SocketUtil::CreateTCPSocket(AF_INET);
	auto address = SocketAddress(INADDR_ANY, 20000);

	listen->bind(address);
	IOService =  std::make_shared<SelectIO>(listen, OnAccept, OnRecv, OnSend, OnExcept);

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

	if (bytes == 0)
	{
		std::cout << "Client Disconnect" << std::endl;

		auto iter = std::find(_clientSocketList.begin(), _clientSocketList.end(), socket);
		if (iter == _clientSocketList.end())
			return;

		_clientSocketList.erase(iter);
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
	_clientSocketList.push_back(socket);
}
void OnSend(TcpSocketPtr socket)
{

}

void OnExcept(TcpSocketPtr socket)
{

}
