#pragma once

#include "Precompile.h"
#include "DBTask.h"
#include "DBTaskQueue.h"
#include "DBConnectionService.h"

namespace App
{ 
	namespace DataBase = Util::DB;

	typedef std::queue<DataBase::DBTask *> DBTaskWorkSerializedQueue;

	template <class InheritanceDBConnectionService>
	class DBInterface abstract
	{
		static_assert(std::is_base_of<DataBase::DBConnectionService, InheritanceDBConnectionService>::value,
			"DBInterface Template is Have to Base of DB::DBConnectionService");

	protected:
		std::shared_ptr<DBConnectionSetting>			_ConnectionSetting;
		std::shared_ptr<InheritanceDBConnectionService>		_Service;

		DataBase::DBTaskQueue*						   _TaskQueue;
	
		static std::atomic_int8_t				   _DataBaseCounter;
		std::atomic_uint32_t					   _DBTaskIdentityCounter = 0;

		NON_COPYABLE(DBInterface<InheritanceDBConnectionService>);
	public:
		DBInterface(const std::string& dbmsname, const std::string& ip, const uint16_t port,
			const std::string& userid, const std::string& passwd,
			const std::string& databaseName, 
			const DataBase::DBTaskQueue* pTaskQueue, const uint32_t NumOfCommand = 10);
		virtual ~DBInterface() = default;

		void Initialize();

		template <class InheritanceDBTask>
		InheritanceDBTask* CreateDBTask(const std::string& TaskName, const std::string& Query);

		template <class InheritanceDBTask>
		void ProcessASyncDB(const InheritanceDBTask* pTask);
	};

	template<class InheritanceDBConnectionService>
	std::atomic_int8_t DBInterface<InheritanceDBConnectionService>::_DataBaseCounter = 0;

	template<class InheritanceDBConnectionService>
	inline DBInterface<InheritanceDBConnectionService>::DBInterface(const std::string& dbmsname, const std::string& ip, const uint16_t port,
		const std::string& userid, const std::string& passwd, const std::string& databaseName,
		const DataBase::DBTaskQueue* pTaskQueue, const uint32_t NumOfCommand)
		:_TaskQueue(const_cast<DataBase::DBTaskQueue *>(pTaskQueue))
	{
		_ConnectionSetting = std::make_shared<DBConnectionSetting>();

		_ConnectionSetting->_DBName = dbmsname;
		_ConnectionSetting->_Host = ip;
		_ConnectionSetting->_Port = port;
		_ConnectionSetting->_User = userid;
		_ConnectionSetting->_Passwd = passwd;
		_ConnectionSetting->_db = databaseName;
		_ConnectionSetting->_DBNameIndex = _DataBaseCounter++;

		_Service = std::make_shared<InheritanceDBConnectionService>(NumOfCommand, _ConnectionSetting.get());

	}

	template<class InheritanceDBConnectionService>
	inline void DBInterface<InheritanceDBConnectionService>::Initialize()
	{
		if (nullptr == _TaskQueue)
		{
			throw Util::Exception(-1, "TaskQueue is NullPointer", __FILE__, __LINE__, __FUNCTION__);
		}

		if (!_TaskQueue->AddDBProcessor(_Service.get()))
		{
			throw Util::Exception(-1, "TaskQueue Add DB Processor", __FILE__, __LINE__, __FUNCTION__);
		}
	}


	template<class InheritanceDBConnectionService>
	template<class InheritanceDBTask>
	inline InheritanceDBTask* DBInterface<InheritanceDBConnectionService>::CreateDBTask(const std::string& TaskName, const std::string& Query)
	{
		static_assert(std::is_base_of<DataBase::DBTask, InheritanceDBTask>::value,
			"InheritanceDBTask is Have to Base of DB::DBTask");

		InheritanceDBTask* pGeneratePtr = new InheritanceDBTask(_DBTaskIdentityCounter, _ConnectionSetting->_DBNameIndex,
			TaskName, Query, "");

		return pGeneratePtr;
	}

	template<class InheritanceDBConnectionService>
	template<class InheritanceDBTask>
	inline void DBInterface<InheritanceDBConnectionService>::ProcessASyncDB(const InheritanceDBTask* pTask)
	{
		static_assert(std::is_base_of<DataBase::DBTask, InheritanceDBTask>::value,
			"InheritanceDBTask is Have to Base of DB::DBTask");

		if (nullptr == _TaskQueue)
		{
			throw Util::Exception(-1, "TaskQueue is NullPointer", __FILE__, __LINE__, __FUNCTION__);
		}

		DBTaskWorkSerializedQueue* Queue = new DBTaskWorkSerializedQueue;
		Queue->push(reinterpret_cast<DataBase::DBTask *>(const_cast<InheritanceDBTask *>(pTask)));

		_TaskQueue->EnqueueDBTask(Queue);
	}

#define COMMON_DBTASK_ARGUMENT									\
	const uint32_t id, const uint32_t dbNameIndex,				\
	const std::string& name, const std::string& dbCommandName,	\
	const std::string& keyPrefix								\

#define COMMON_DBTASK_CALLCTOR	DataBase::DBTask(id, dbNameIndex, name, dbCommandName, keyPrefix)

}
