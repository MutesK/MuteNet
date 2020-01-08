#pragma once

#include "DBInterface.h"
#include "MariaDBConnectionService.h"

namespace App
{

	class TestDBInterface : public DBInterface<DataBase::MariaDBConnectionService>
	{
		typedef DBInterface<DataBase::MariaDBConnectionService> super;
	public:
		TestDBInterface();
		virtual ~TestDBInterface() = default;

		void CreateTemperClass();
		void InsertTemperClass(const std::string& InsertData);
		void GetTemperClass();
	};
}
