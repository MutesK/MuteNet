#pragma once

#include "foundation.h"

class PipeImpl;
class Pipe
{
public:
	Pipe();
	Pipe(const std::shared_ptr<PipeImpl>& pipe);
	Pipe& operator=(const Pipe& pipe);
	~Pipe();

	void close();

	HANDLE readPipe();
	HANDLE writePipe();
private:
	std::shared_ptr<PipeImpl>	_pImpl;
};

