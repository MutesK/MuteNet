#include "Select.h"

SelectIO::SelectIO(TcpSocketPtr& listen, std::function<void(TcpSocketPtr)> Accept,
	std::function<void(TcpSocketPtr)> Recv, std::function<void(TcpSocketPtr)> Send,
	std::function<void(TcpSocketPtr)> Except)
	: _listen(listen), _OnAccpet(Accept), _OnRecv(Recv), _OnSend(Send),
	_OnExcept(Except), Thread()
{
}

fd_set* SelectIO::InFillFDSet(int mode)
{
	fd_set target{0};

	switch (mode)
	{
	case eReadMode:
		target = read;
		break;
	case eWriteMode:
		target = write;
		break;
	case eExcpetMode:
		target = except;
		break;
	}

	if (_inSession.size() <= 0)
		return nullptr;

	FD_ZERO(&target);

	for (auto& mapiter : _inSession)
	{
		std::shared_ptr<TcpSocket>& socket = mapiter.second;

		FD_SET(socket->get_socket(), &target);
	}

	return &target;
}

void SelectIO::OutFillFDSet(int mode)
{
	fd_set target;
	std::map<__int64, TcpSocketPtr>* _outSession = nullptr;

	switch (mode)
	{
	case eReadMode:
		target = read;
		_outSession = &_OutReadSet;
		break;
	case eWriteMode:
		target = write;
		_outSession = &_OutWrtieSet;
		break;
	case eExcpetMode:
		target = except;
		_outSession = &_OutExceptSet;
		break;

	}
	if (_inSession.size() == 0 && _outSession->size() == 0)
		return;

	_outSession->clear();
	for (const auto& iter : _inSession)
	{
		const TcpSocketPtr& socket = iter.second;
		
		if (FD_ISSET(socket->get_socket(), &target))
			(*_outSession)[socket->get_socket()] = socket;
	}

}

int SelectIO::Select()
{
	fd_set *pRead = InFillFDSet(eReadMode);
	fd_set *pWrite = InFillFDSet(eWriteMode);
	fd_set *pExcept = InFillFDSet(eExcpetMode);

	int result = select(0, pRead, pWrite, pExcept, nullptr);

	if (result <= 0)
		return result;

	OutFillFDSet(eReadMode);
	OutFillFDSet(eWriteMode);
	OutFillFDSet(eExcpetMode);

	ReadSetCallback();
	WriteSetCallback();
	ExcpetSetCallback();

}

void SelectIO::ReadSetCallback()
{
	for (auto readiter : _OutReadSet)
	{
		if (readiter.second == _listen)
		{
			_OnAccpet(readiter.second);
		}
		else
			_OnRecv(readiter.second);

	}
}
void SelectIO::WriteSetCallback()
{
	for (auto writeIter : _OutWrtieSet)
	{
		_OnSend(writeIter.second);
	}
}
void SelectIO::ExcpetSetCallback()
{
	for (auto writeIter : _OutWrtieSet)
	{
		_OnExcept(writeIter.second);
	}
}

void SelectIO::DoWork()
{
	while (!_isthreadwork)
	{
		Select();
	}
}

void SelectIO::EmitWakeupSignal()
{

}
void SelectIO::EnqueueSocket(TcpSocketPtr& ptr)
{
	_inSession[ptr->get_socket()] = ptr;
}

void SelectIO::DequeueSocket(TcpSocketPtr& ptr)
{
	_inSession.erase(ptr->get_socket());
}