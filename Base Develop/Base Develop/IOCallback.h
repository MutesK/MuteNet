#pragma once

#include <atomic>

class IOCallback : OVERLAPPED
{
public:
	IOCallback();
	~IOCallback();

	GET_SET_ATTRIBUTE(std::atomic<size_t>&, referenceCount);

private:
	std::atomic<size_t>	_referenceCount;
};

