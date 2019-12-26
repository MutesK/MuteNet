#pragma once

#include "DBConnectionSetting.h"

namespace Util
{
	namespace DB
	{
		class DBConnection;
		class DBConnectionService final
		{
			uint32_t _NumOfCommands;
			DBConnectionSetting* _pConnectionSetting;

		public:
			DBConnectionService(const uint32_t NumOfCommand,
				DBConnectionSetting* const pConnectionSetting);

			uint32_t GetNumOfCommand() const;
			std::string GetDBName() const;
			uint32_t GetDBNameIndex() const;

			DBConnection* AcquireConnection();
			void ReleaseConnection(DBConnection* const pConnection);

		protected:
			DBConnection* AcquireConnectionImpl();
			virtual DBConnection* NewConnection() = 0;
		};

		inline uint32_t DBConnectionService::GetNumOfCommand() const
		{
			return _NumOfCommands;
		}
		inline std::string DBConnectionService::GetDBName() const
		{
			return _pConnectionSetting->_DBName;
		}
		inline uint32_t DBConnectionService::GetDBNameIndex() const
		{
			return _pConnectionSetting->_DBNameIndex;
		}
	}
}