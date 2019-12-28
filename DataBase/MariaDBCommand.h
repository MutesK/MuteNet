#pragma once

#include "DBCommand.h"

namespace Util
{
	namespace DB
	{
		class MariaDBInputParameterCollection;
		class MariaDBCommand : public DBCommand
		{
			typedef DBCommand super;

			std::shared_ptr<MYSQL_STMT> _pMySQLStatement;
			std::shared_ptr<MariaDBInputParameterCollection> _pParameters;
		public:
			MariaDBCommand(DBConnection* const pConnection,
				const std::shared_ptr<MYSQL_STMT>& pMySQLStatement,
				const std::string& Query);
			virtual ~MariaDBCommand();

			MYSQL_STMT* GetStatement();
			void Refresh();

			// DBCommand을(를) 통해 상속됨
			virtual std::shared_ptr<DBResultSet> Execute() override;
			virtual void Close() override;

			virtual void Bind(const uint8_t& value) override;

			virtual void Bind(const uint16_t& value) override;

			virtual void Bind(const uint32_t& value) override;

			virtual void Bind(const uint64_t& value) override;

			virtual void Bind(const int8_t& value) override;

			virtual void Bind(const int16_t& value) override;

			virtual void Bind(const int32_t& value) override;

			virtual void Bind(const int64_t& value) override;

			virtual void Bind(const double& value) override;

			virtual void Bind(const wchar_t* value, const uint32_t length) override;

			virtual void PrepareImpl() override;

			virtual DBResultSet* NewResultSetImpl() override;

			virtual void ResetImpl() override;

		};

		inline MYSQL_STMT* MariaDBCommand::GetStatement()
		{
			return _pMySQLStatement.get();
		}
	}
}