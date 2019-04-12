#pragma once

#include "foundation.h"

namespace ODBC
{
	class Connection final
	{
	public:
		Connection();
		~Connection();

		void Connect(std::string& dsn, std::string& userid, std::string& password, size_t timeout = 0);
		void Connect(std::string& connect_string, size_t timeout = 0);

		bool Disconnect();

		void* native_env_handle() { return _henv; }
		void* native_dbc_handle() { return _hdbc; }

		bool isConnected() { return _connected; }
	private:
		void AllocHandle();
		void DeallocHandle();
	private:
		SQLHENV _henv;
		SQLHDBC _hdbc;

		bool	_connected;
	};
};
