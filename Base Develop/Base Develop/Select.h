#pragma once

#include "../Foundation/foundation.h"
#include "../Foundation/Thread.h"
#include "SocketUtil.h"

/*
	SelectIO Select File Descriptor Set ��  Select ���� ������� ���� ó�����ִ� ����.

	_inSession�� �񵿱������� Enqueue, Dequeue�ϹǷ�, �̹� ������ �����ε�, �ݹ��� �ü��� ����.
	-> �������� ������� �ʴ´ٸ�, �Ⱦ��°� ��õ. (�ұԸ� ��Ʈ��ũ�� ������)

	Map -> Mutex �߰�����
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

