#pragma once
class IOCallback
{
public:
	IOCallback();
	~IOCallback();

	GET_SET_ATTRIBUTE(OVERLAPPED&, InputOverlapped);
	GET_SET_ATTRIBUTE(OVERLAPPED&, OutputOverlapped);
private:
	OVERLAPPED  _InputOverlapped;
	OVERLAPPED  _OutputOverlapped;
};

