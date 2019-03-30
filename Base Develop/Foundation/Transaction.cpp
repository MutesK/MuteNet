#include "foundation.h"
#include "Transaction.h"
#include "Connection.h"

using namespace ODBC;

Transaction::Transaction(const Connection& conn)
	:_conn(conn), _commit(false)
{
	// Transaction Ω√¿€
	assert(_conn.isConnected());

	TurnoffAutoCommit();
}


Transaction::~Transaction()
{
	if (!_commit)
		Rollback();

	TurnonAutoCommit();
}


void Transaction::Commit()
{
	_commit = true;
	SQLEndTran(SQL_HANDLE_DBC, _conn.native_dbc_handle(), SQL_COMMIT);
}

void Transaction::Rollback()
{
	SQLEndTran(SQL_HANDLE_DBC, _conn.native_dbc_handle(), SQL_ROLLBACK);
}

void Transaction::TurnoffAutoCommit()
{
	RETCODE rc = SQLSetConnectAttr(_conn.native_dbc_handle(),
		SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_IS_UINTEGER);
	CHECK_AND_EXCEPT(rc);
}

void Transaction::TurnonAutoCommit()
{
	RETCODE rc = SQLSetConnectAttr(_conn.native_dbc_handle(),
		SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_IS_UINTEGER);
	CHECK_AND_EXCEPT(rc);
}