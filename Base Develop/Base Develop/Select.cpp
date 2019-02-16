#include "Select.h"

SelectIO::SelectIO(TcpSocketPtr& listen, std::function<void(TcpSocketPtr)>&& Accept,
	std::function<void(TcpSocketPtr)>&& Recv, std::function<void(TcpSocketPtr)>&& Send,
	std::function<void(TcpSocketPtr)>&& Except)
	: _listen(listen), _OnAccpet(std::move(Accept)), _OnRecv(std::move(Recv)), _OnSend(std::move(Send)),
	_OnExcept(std::move(Except)), Thread()
{
	EnqueueSocket(listen);
}

SelectIO::SelectIO(TcpSocketPtr& clientsock,
	std::function<void(TcpSocketPtr)>&& Recv, std::function<void(TcpSocketPtr)>&& Send,
	std::function<void(TcpSocketPtr)>&& Except)
	: _OnRecv(std::move(Recv)), _OnSend(std::move(Send)),
	_OnExcept(std::move(Except)), Thread()
{
	EnqueueSocket(clientsock);

}
void SelectIO::InFillFDSet(int mode)
{
	fd_set* target = nullptr;

	switch (mode)
	{
	case eReadMode:
		target = &read;
		break;
	case eWriteMode:
		target = &write;
		break;
	case eExcpetMode:
		target = &except;
		break;
	}

	FD_ZERO(target);

	if (_inSession.size() <= 0)
		return;

	for (auto& mapiter : _inSession)
	{
		std::shared_ptr<TcpSocket>& socket = mapiter.second;

		FD_SET(socket->get_socket(), target);
	}
}

void SelectIO::OutFillFDSet(int mode)
{
	fd_set* target = nullptr;
	std::map<__int64, TcpSocketPtr>* _outSession = nullptr;

	switch (mode)
	{
	case eReadMode:
		target = &read;
		_outSession = &_OutReadSet;
		break;
	case eWriteMode:
		target = &write;
		_outSession = &_OutWrtieSet;
		break;
	case eExcpetMode:
		target = &except;
		_outSession = &_OutExceptSet;
		break;

	}
	_outSession->clear();

	for (const auto& iter : _inSession)
	{
		const TcpSocketPtr& socket = iter.second;
		
		if (FD_ISSET(socket->get_socket(), target))
			(*_outSession)[socket->get_socket()] = socket;
	}

}

int SelectIO::Select()
{
	InFillFDSet(eReadMode);
	InFillFDSet(eWriteMode);
	InFillFDSet(eExcpetMode);

	int result = select(0, &read, &write, &except, nullptr);

	if (result <= 0)
		return result;

	OutFillFDSet(eReadMode);
	OutFillFDSet(eWriteMode);
	OutFillFDSet(eExcpetMode);

	ReadSetCallback();
	WriteSetCallback();
	ExcpetSetCallback();

	return result;
}

void SelectIO::ReadSetCallback()
{
	for (auto readiter : _OutReadSet)
	{
		if (readiter.second == _listen)
		{
			_OnAccpet(readiter.second->Accept());
		}
		else
			_OnRecv(readiter.second);
	}

	_OutReadSet.clear();
}
void SelectIO::WriteSetCallback()
{
	for (auto writeIter : _OutWrtieSet)
	{
		_OnSend(writeIter.second);
	}
	_OutWrtieSet.clear();
}
void SelectIO::ExcpetSetCallback()
{
	for (auto writeIter : _OutWrtieSet)
	{
		_OnExcept(writeIter.second);
	}
	_OutExceptSet.clear();
}

void SelectIO::DoWork()
{
	while (_isthreadwork)
	{
		Select();
	}
}

void SelectIO::EmitWakeupSignal()
{
	_isthreadwork = !_isthreadwork;
}
void SelectIO::EnqueueSocket(TcpSocketPtr& ptr)
{
	_inSession[ptr->get_socket()] = ptr;
}

void SelectIO::DequeueSocket(TcpSocketPtr& ptr)
{
	_inSession.erase(ptr->get_socket());
}