
#include "Statement.h"
#include "Connection.h"
#include "Transaction.h"

using namespace ODBC;

Statement::~Statement()
{
}

// 기본 자료형이 아닌 경우, 특수화 필수
// Date, Time 쪽 추가해야됨.
Statement::Statement()
	:_stmt(0) ,_open(false)
{
	_CTypeConvert[typeid(unsigned char *).hash_code()] = SQL_C_CHAR;
	_CTypeConvert[typeid(wchar_t *).hash_code()] = SQL_C_WCHAR;
	_CTypeConvert[typeid(short int).hash_code()] = SQL_C_SSHORT;
	_CTypeConvert[typeid(unsigned short int).hash_code()] = SQL_C_USHORT;
	_CTypeConvert[typeid(long int).hash_code()] = SQL_C_SLONG;
	_CTypeConvert[typeid(unsigned long int).hash_code()] = SQL_C_ULONG;
	_CTypeConvert[typeid(float).hash_code()] = SQL_C_FLOAT;
	_CTypeConvert[typeid(double).hash_code()] = SQL_C_DOUBLE;
	_CTypeConvert[typeid(unsigned char).hash_code()] = SQL_C_BIT;
	_CTypeConvert[typeid(bool).hash_code()] = SQL_C_BIT;
	_CTypeConvert[typeid(signed char).hash_code()] = SQL_C_STINYINT;
	
	_SQLTypeConvert[typeid(unsigned char *).hash_code()] = SQL_CHAR;
	_SQLTypeConvert[typeid(wchar_t *).hash_code()] = SQL_WCHAR;
	_SQLTypeConvert[typeid(short int).hash_code()] = SQL_SMALLINT;
	_SQLTypeConvert[typeid(unsigned short int).hash_code()] = SQL_SMALLINT;
	_SQLTypeConvert[typeid(long int).hash_code()] = SQL_INTEGER;
	_SQLTypeConvert[typeid(unsigned long int).hash_code()] = SQL_INTEGER;
	_SQLTypeConvert[typeid(float).hash_code()] = SQL_REAL;
	_SQLTypeConvert[typeid(double).hash_code()] = SQL_FLOAT;
	_SQLTypeConvert[typeid(unsigned char).hash_code()] = SQL_BIT;
	_SQLTypeConvert[typeid(bool).hash_code()] = SQL_BIT;
	_SQLTypeConvert[typeid(signed char).hash_code()] = SQL_TINYINT;

}

bool Statement::Open(class Connection& conn)
{
	Close();
	RETCODE rc = SQLAllocHandle(SQL_HANDLE_STMT, conn.native_dbc_handle(), &_stmt);
	CHECK_AND_EXCEPT(rc);
	_open = true;

	_conn = conn;
	return _open;
}

bool Statement::Close()
{
	if (_open && _conn.isConnected())
	{
		RETCODE rc = SQLCancel(_stmt);
		CHECK_AND_EXCEPT(rc);

		Reset_parameter();

		rc = SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
		CHECK_AND_EXCEPT(rc);

		_stmt = nullptr;
		_open = false;
		return true;
	}

	return false;
}

void Statement::Reset_parameter()
{
	SQLFreeStmt(_stmt, SQL_RESET_PARAMS);
}

void Statement::Prepare(const std::string& query, size_t timeout)
{
	if (_open)
		throw std::runtime_error("Statement has no effective connection object");

	RETCODE rc = SQLPrepare(_stmt,
		(SQLCHAR *)query.c_str(), (SQLINTEGER)query.size());
	CHECK_AND_EXCEPT(rc);

	if (timeout >= 0)
		this->Timeout(timeout);
}

void Statement::Timeout(size_t timeout)
{
	RETCODE rc = SQLSetStmtAttr(_stmt, SQL_ATTR_QUERY_TIMEOUT,
		(SQLPOINTER)(std::intptr_t)timeout, 0);

	CHECK_AND_EXCEPT(rc);
}

void Statement::Execute()
{
	RETCODE rc = SQLExecute(_stmt);
	CHECK_AND_EXCEPT(rc);
}

void Statement::ExecuteDirect(const std::string& query, size_t timeout)
{
	RETCODE rc = SQLExecDirect(_stmt,
		(SQLCHAR *)query.c_str(), (SQLINTEGER)query.size());

	CHECK_AND_EXCEPT(rc);


	if (timeout >= 0)
		this->Timeout(timeout);
}

short Statement::CountParameter()
{
	SQLSMALLINT Params;
	RETCODE rc = SQLNumParams(_stmt, &Params);
	CHECK_AND_EXCEPT(rc);

	return Params;
}


void Statement::Cancle()
{
	RETCODE rc = SQLCancel(_stmt);
	CHECK_AND_EXCEPT(rc);
}