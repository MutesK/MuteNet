#pragma once

#include "DBResultSet.h"

namespace Util
{
	namespace DB
	{
		class MariaDBCommand;
		class MariaDBOutputParameterCollection;
		class MariaDBResultSet final : public DBResultSet
		{
			MariaDBCommand* _pDBCommand;
			std::shared_ptr<MariaDBOutputParameterCollection> _pParameters;
		public:
			MariaDBResultSet(MariaDBCommand* const pDBCommand);
			virtual ~MariaDBResultSet();


			// DBResultSet을(를) 통해 상속됨
			virtual void BindImpl() override;

			virtual void ResetImpl() override;

			virtual bool NextResultSetImpl() override;

			virtual bool NextImpl() override;

			virtual void Bind(uint8_t& OUT value) override;

			virtual void Bind(uint16_t& OUT value) override;

			virtual void Bind(uint32_t& OUT value) override;

			virtual void Bind(uint64_t& OUT value) override;

			virtual void Bind(int8_t& OUT value) override;

			virtual void Bind(int16_t& OUT value) override;

			virtual void Bind(int32_t& OUT value) override;

			virtual void Bind(int64_t& OUT value) override;

			virtual void Bind(double& OUT value) override;

			virtual void Bind(char* OUT value, uint32_t& OUT length, uint32_t maxLength) override;

			virtual void GetResult(int16_t& OUT result) override;

			virtual uint64_t GetNumberOfRows() const override;

		};
	}
}

