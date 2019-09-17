#include "Statement.h"

Statement::Statement(const SQLHDBC DBCHandle)
{
	_lastReturn = SQLAllocHandle(SQL_HANDLE_STMT, DBCHandle, &_stmt);
}

Statement::~Statement()
{
	if(_stmt)
		_lastReturn = SQLFreeHandle(SQL_HANDLE_STMT, _stmt);
}

SQLRETURN Statement::ExecuteQuery(char* query)
{
	const auto ret = SQLExecDirect(_stmt, reinterpret_cast<SQLCHAR*>(query), SQL_NTS);
	_lastReturn = ret;

	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return ret;
	else
		throw ret;
}

auto Statement::PrepareQuery(char* query) -> SQLRETURN
{
	const auto ret = SQLPrepare(_stmt, reinterpret_cast<SQLCHAR *>(query), SQL_NTS);
}

SQLRETURN Statement::Fetch()
{
	const auto ret = SQLFetch(_stmt);
	_lastReturn = ret;

	if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO)
		throw ret;

	return ret;
}

void Statement::GetData(short& value, int ColumnNum)
{
	_lastReturn = SQLGetData(_stmt, ColumnNum, 
		SQL_C_SHORT, &value, sizeof(short), nullptr);
}

void Statement::GetData(int& value, int ColumnNum)
{
	_lastReturn = SQLGetData(_stmt, ColumnNum,
		SQL_C_LONG, &value, sizeof(int), nullptr);
}

void Statement::GetData(float& value, int ColumnNum)
{
	_lastReturn = SQLGetData(_stmt, ColumnNum,
		SQL_C_FLOAT, &value, sizeof(float), nullptr);
}

void Statement::GetData(double& value, int ColumnNum)
{
	_lastReturn = SQLGetData(_stmt, ColumnNum,
		SQL_C_DOUBLE, &value, sizeof(double), nullptr);
}

void Statement::GetData(std::string& value, int ColumnNum)
{
	char buffer[10000] {'\0'};

	_lastReturn = SQLGetData(_stmt, ColumnNum,
		SQL_C_CHAR, &buffer, 25000, nullptr);

	value = buffer;
}

auto Statement::BindParameter(int ColumnNum, const short& value) -> SQLRETURN
{

}
