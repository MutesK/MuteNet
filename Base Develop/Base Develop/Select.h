#pragma once

#include "../Foundation/foundation.h"
#include "../Foundation/Thread.h"
#include "SocketUtil.h"

/*
	SelectIO Select File Descriptor Set 과  Select 콜을 스레드로 따로 처리해주는 구조.

	_inSession에 비동기적으로 Enqueue, Dequeue하므로, 이미 끊어진 소켓인데, 콜백이 올수도 있음.
	-> 봇용으로 사용하지 않는다면, 안쓰는걸 추천. (소규모 네트워크에 사용권장)

	Map -> Mutex 추가예정
*/
enum SetMode
{
	eReadMode,
	eWriteMode,
	eExcpetMode
};

class SelectIO : public Thread
{
public:
	SelectIO(TcpSocketPtr& listen, std::function<void(TcpSocketPtr)>&& Accept,
		std::function<void(TcpSocketPtr)>&& Recv, std::function<void(TcpSocketPtr)>&& Send,
		std::function<void(TcpSocketPtr)>&& Except);

	SelectIO(TcpSocketPtr& clientsock,
		std::function<void(TcpSocketPtr)>&& Recv, std::function<void(TcpSocketPtr)>&& Send,
		std::function<void(TcpSocketPtr)>&& Except);

	void EnqueueSocket(TcpSocketPtr& ptr);
	void DequeueSocket(TcpSocketPtr& ptr);
private:
	void InFillFDSet(int mode);
	void OutFillFDSet(int mode);

	void ReadSetCallback();
	void WriteSetCallback();
	void ExcpetSetCallback();

	int Select();

private:
	TcpSocketPtr _listen;


	std::map<__int64, TcpSocketPtr> _inSession;
	
	std::map<__int64, TcpSocketPtr> _OutReadSet;
	std::map<__int64, TcpSocketPtr> _OutWrtieSet;
	std::map<__int64, TcpSocketPtr> _OutExceptSet;


	std::function<void(TcpSocketPtr)>    _OnAccpet;
	std::function<void(TcpSocketPtr)>    _OnRecv;
	std::function<void(TcpSocketPtr)>	_OnSend;
	std::function<void(TcpSocketPtr)>	_OnExcept;

	int    _lastError;
	fd_set read;
	fd_set write;
	fd_set except;
protected:
	virtual void DoWork() override;


	virtual void EmitWakeupSignal() override;

};

