#pragma once

#include "DBConnectionService.h"

namespace Util
{
	namespace DB
	{
		class MariaDBConnectionService final : public DBConnectionService
		{
			typedef DBConnectionService super;
		public:
			MariaDBConnectionService(const uint32_t numOfCommand, DBConnectionSetting* const pConnectionSetting);
			virtual ~MariaDBConnectionService() = default;

			// DBConnectionService을(를) 통해 상속됨
			virtual DBConnection* NewConnection() override;
		};
	}
}

