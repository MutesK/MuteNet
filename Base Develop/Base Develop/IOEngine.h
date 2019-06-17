#pragma once

/*
*/
class IOEngine
{
public:
	void SetOnAccept(void* Context);
	void SetOnDisconnect(void* Context);
	void SetOnConnect(void* Context);
	void SetOnRecived(void* Context);
	void SetOnSended(void* Context);
};

