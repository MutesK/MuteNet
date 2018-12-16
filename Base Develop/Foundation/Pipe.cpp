#include "Pipe.h"
#include "PipeImpl.h"


Pipe::Pipe()
{
	_pImpl = std::make_shared<PipeImpl>();
}

Pipe::Pipe(const std::shared_ptr<PipeImpl>& pipe)
{
	_pImpl = pipe;
}

Pipe& Pipe::operator=(const Pipe& pipe)
{
	if(_pImpl != pipe._pImpl)
		this->_pImpl = pipe._pImpl;

	return *this;
}

Pipe::~Pipe()
{
}

void Pipe::close()
{
	_pImpl->closePipe();
}


HANDLE Pipe::readPipe()
{
	return _pImpl->readHandle();
}
HANDLE Pipe::writePipe()
{
	return _pImpl->writeHandle();
}