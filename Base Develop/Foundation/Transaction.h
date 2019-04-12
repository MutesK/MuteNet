#pragma once

#include "foundation.h"
#include "Connection.h"

namespace ODBC
{
	class Transaction final
	{
	public:
		Transaction(const Connection& conn);
		~Transaction();

		void Commit();
	private:
		// Auto Commit Off
		void TurnoffAutoCommit();
		void TurnonAutoCommit();

		void Rollback();
	private:
		Connection _conn;
		bool _commit;
	};
}