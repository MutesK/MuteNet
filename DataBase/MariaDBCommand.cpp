#include "MuteNetDB.h"
#include "MariaDBCommand.h"
#include "MariaDBParameterCollection.h"
#include "MariaDBConnection.h"
#include "MariaDBResultSet.h"

namespace Util
{
	namespace DB
	{

		MariaDBCommand::MariaDBCommand(DBConnection* const pConnection,
			const std::shared_ptr<MYSQL_STMT>& pMySQLStatement, const std::string& Query)
			:super(pConnection, Query),
			_pMySQLStatement(nullptr)
		{
			_pParameters = std::make_shared<MariaDBInputParameterCollection>();
		}

		MariaDBCommand::~MariaDBCommand()
		{
			if (nullptr != _pMySQLStatement)
			{
				::mysql_stmt_close(GetStatement());
			}
		}


		void MariaDBCommand::Refresh()
		{
			if (nullptr != _pMySQLStatement)
			{
				::mysql_stmt_close(GetStatement());
			}

			ResetImpl();

			const auto pConnection = reinterpret_cast<MariaDBConnection *>(_pConnection);
			_pMySQLStatement = pConnection->CreateStatement();

			PrepareImpl();
		}

		std::shared_ptr<DBResultSet> MariaDBCommand::Execute()
		{
			if (nullptr != _pResultSet || nullptr != _pParameters)
			{
				throw;
			}

			_pParameters->Bind(GetStatement());
			const auto result = ::mysql_stmt_execute(GetStatement());

			if (0 != result)
			{
				throw;
			}

			return _pResultSet;
		}

		void MariaDBCommand::Close()
		{
			Reset();

			if (nullptr != _pMySQLStatement)
			{
				::mysql_stmt_close(GetStatement());
			}
		}

		void MariaDBCommand::Bind(const uint8_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_TINY,
				const_cast<uint8_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const uint16_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_SHORT,
				const_cast<uint16_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const uint32_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONG,
				const_cast<uint32_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const uint64_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONGLONG,
				const_cast<uint64_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const int8_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_TINY,
				const_cast<int8_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const int16_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_SHORT,
				const_cast<int16_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const int32_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONG,
				const_cast<int32_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const int64_t& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_LONGLONG,
				const_cast<int64_t*>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(const double& value)
		{
			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_DOUBLE,
				const_cast<double *>(&value), nullptr, sizeof(value), sizeof(value), true);
		}

		void MariaDBCommand::Bind(char* value, const uint32_t maxLength)
		{
			const uint32_t length = static_cast<uint32_t>(::strlen(value));

			_pParameters->AddParameter(enum_field_types::MYSQL_TYPE_VARCHAR,
				const_cast<char*>(value), nullptr, length, maxLength, false);
		}

		void MariaDBCommand::PrepareImpl()
		{
			if (nullptr != _pMySQLStatement)
			{
				throw;
			}

			const uint32_t length = static_cast<uint32_t>(_Query.length());
			const int32_t result = ::mysql_stmt_prepare(GetStatement(),
				_Query.c_str(), length);

			if (0 != result)
			{
				throw; // API Error
			}

			const uint32_t numberOfParameters = ::mysql_stmt_param_count(_pMySQLStatement.get());
			_pParameters->SetNumberOfParameter(numberOfParameters);
		}

		DBResultSet* MariaDBCommand::NewResultSetImpl()
		{
			return new MariaDBResultSet(this);
		}

		void MariaDBCommand::ResetImpl()
		{
			if (nullptr != _pParameters)
			{
				_pParameters->Reset();
			}
		}

	}
}