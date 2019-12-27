#pragma once

#include "DBConnectionSetting.h"

namespace Util
{
	namespace DB
	{
		class DBConnection;
		class DBConnectionService abstract
		{
		protected:
			uint32_t _NumOfCommands;
			DBConnectionSetting* _pConnectionSetting;
		public:
			DBConnectionService(const uint32_t NumOfCommand,
				DBConnectionSetting* const pConnectionSetting);

			uint32_t GetNumOfCommand() const;
			std::string GetDBName() const;
			uint32_t GetDBNameIndex() const;
			DBConnectionSetting* GetSetting() const;

			std::shared_ptr<DBConnection> AcquireConnection();

		protected:
			std::shared_ptr<DBConnection> AcquireConnectionImpl();

			// Automatically Managed Point
			// Don't Call Delete DBConnection
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
		inline DBConnectionSetting* DBConnectionService::GetSetting() const
		{
			return _pConnectionSetting;
		}
	}
}