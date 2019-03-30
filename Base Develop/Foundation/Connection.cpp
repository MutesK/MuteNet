#include "Connection.h"
#include "foundation.h"

using namespace ODBC;

Connection::Connection()
{
	_henv = nullptr;
	_hdbc = nullptr;

	_connected = false;
}


Connection::~Connection()
{
}


void Connection::Connect(std::string& dsn, std::string& userid, std::string& password, size_t timeout)
{
	Disconnect();

	AllocHandle();

	RETCODE rc;
	if (timeout != 0)
	{
		rc = SQLSetConnectAttr(_hdbc, SQL_LOGIN_TIMEOUT,
			(SQLPOINTER)(std::intptr_t)timeout, 0);
		CHECK_AND_EXCEPT(rc);
	}

	rc = SQLConnect(_hdbc,
		(SQLCHAR *)dsn.c_str(), SQL_NTS,
		!userid.empty() ? (SQLCHAR *)userid.c_str() : 0, SQL_NTS,
		!password.empty() ? (SQLCHAR *)password.c_str() : 0, SQL_NTS);
	CHECK_AND_EXCEPT(rc);

	_connected = true;
}

void Connection::Connect(std::string& connect_string, size_t timeout)
{
	Disconnect();

	AllocHandle();

	RETCODE rc;
	if (timeout != 0)
	{
		rc = SQLSetConnectAttr(_hdbc, SQL_LOGIN_TIMEOUT,
			(SQLPOINTER)(std::intptr_t)timeout, 0);
		CHECK_AND_EXCEPT(rc);
	}

	rc = SQLDriverConnect(_hdbc, 0, (SQLCHAR *)connect_string.c_str(), SQL_NTS,
		nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT);

	if (rc == SQL_STILL_EXECUTING)
		CHECK_AND_EXCEPT(rc);

	_connected = true;
}

void Connection::AllocHandle()
{
	RETCODE ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);
	CHECK_AND_EXCEPT(ret);

	ret = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
	CHECK_AND_EXCEPT(ret);
}

void Connection::DeallocHandle()
{
	RETCODE rc;

	if (_henv)
	{
		rc = SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		CHECK_AND_EXCEPT(rc);

	}
	if (_hdbc)
	{
		rc = SQLFreeHandle(SQL_HANDLE_DBC, _hdbc);
		CHECK_AND_EXCEPT(rc);
	}

	_henv = nullptr;
	_hdbc = nullptr;
}

bool Connection::Disconnect()
{
	if (!_connected)
		return false;

	RETCODE rc = SQLDisconnect(_hdbc);
	CHECK_AND_EXCEPT(rc);

	_connected = false;

	return true;
}