#include "MuteNetDB.h"
#include "MariaDBResultSet.h"
#include "MariaDBCommand.h"
#include "MariaDBParameterCollection.h"

namespace Util
{
	namespace DB
	{
		MariaDBResultSet::MariaDBResultSet(MariaDBCommand* const pDBCommand)
			:_pDBCommand(pDBCommand)
		{
			_pParameters = std::make_shared<MariaDBOutputParameterCollection>();
		}

		MariaDBResultSet::~MariaDBResultSet()
		{
		}

		void MariaDBResultSet::BindImpl()
		{
			const auto pStatement = _pDBCommand->GetStatement();

			_pParameters->Bind(pStatement);

			const auto result = ::mysql_stmt_store_result(pStatement);

			if (0 != result)
			{
				throw; // DB COMMAND RESULT
			}
		}

		void MariaDBResultSet::ResetImpl()
		{
			_pParameters->Reset();

			const auto pStatement = _pDBCommand->GetStatement();
			const auto result = ::mysql_stmt_free_result(pStatement);

			if (0 != result)
			{
				throw; // DB COMMAND RESULT
			}
		}

		bool MariaDBResultSet::NextResultSetImpl()
		{
			ResetImpl();

			const auto pStatement = _pDBCommand->GetStatement();
			const int32_t result = ::mysql_stmt_next_result(pStatement);

			if (0 != result)
			{
				if (0 < result)
				{
					throw; 
				}
			}

			const uint32_t numberOfParameters = ::mysql_stmt_field_count(pStatement);
			_pParameters->SetNumberOfParameter(numberOfParameters);

			return 0 < numberOfParameters;
		}

		bool MariaDBResultSet::NextImpl()
		{
			const auto pStatement = _pDBCommand->GetStatement();
			const int32_t result = ::mysql_stmt_fetch(pStatement);

			if (0 != result)
			{
				if (MYSQL_NO_DATA != result)
				{
					throw;
				}

				return false;
			}

			return true;
		}

		void MariaDBResultSet::Bind(uint8_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_TINY,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(uint16_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_SHORT,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(uint32_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONG,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(uint64_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONGLONG,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(int8_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_TINY,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(int16_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_SHORT,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(int32_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONG,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(int64_t& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONGLONG,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(double& OUT value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_DOUBLE,
				&value, nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBResultSet::Bind(char* OUT value, uint32_t& OUT length, uint32_t maxLength)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_STRING,
				value, &length, 0, maxLength, false);
		}

		void MariaDBResultSet::GetResult(int16_t& OUT result)
		{
			const auto pStatement = _pDBCommand->GetStatement();
			const uint32_t numberOfParameters = ::mysql_stmt_field_count(pStatement);

			if (0 == numberOfParameters)
			{
				throw; // No Result Set
			}

			_pParameters->SetNumberOfParameter(numberOfParameters);

			Bind(result);
			BindImpl();

			if (!NextImpl())
			{
				throw; // No Result Set
			}

			while (NextImpl ());

		}

		uint64_t MariaDBResultSet::GetNumberOfRows() const
		{
			const auto pStatement = _pDBCommand->GetStatement();

			return ::mysql_stmt_num_rows(pStatement);
		}




	}
}
