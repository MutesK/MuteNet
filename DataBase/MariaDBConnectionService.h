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

			// DBConnectionService��(��) ���� ��ӵ�
			virtual DBConnection* NewConnection() override;
		};
	}
}

