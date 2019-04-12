#pragma once

#include "foundation.h"
#include "Connection.h"

namespace ODBC
{
	enum ParamTYPE
	{
		INPUT = SQL_PARAM_INPUT,
		OUTPUT = SQL_PARAM_OUTPUT,
		INOUTPUT = SQL_PARAM_INPUT_OUTPUT,
	};
	class Statement final
	{
	public:
		Statement(const Statement&) = delete;
		Statement operator=(Statement&) = delete;

		Statement();
		~Statement();

		bool Open(class Connection& conn);
		bool Close();
		void Cancle();

		bool isOpen() { return _open; }


		void Prepare(const std::string& query, size_t timeout);
		void Execute();
		/*
			저장프로시저 Call용으로 적합
		*/
		void ExecuteDirect(const std::string& query, size_t timeout);

		//// Parameter
		void Reset_parameter();
		short CountParameter();

		template<typename T>
		bool Bind(const T& value, ParamTYPE Type, int index)
		{
			short type;
			auto rc = SQLBindParameter(_stmt, index, Type,
				_CTypeConvert[typeid(value).hash_code()], _SQLTypeConvert[typeid(value).hash_code()],
				0, 0, &value, 0, &SQL_NTS);
			
			if (rc != SQL_SUCCESS)
			{
				return false;
			}

			return true;
		}

		template<typename T>
		void GetValue(volatile T& value, int index)
		{
			SQLLEN cbNum = SQL_NTS;

			SQLRETURN ret = SQLGetData(_stmt, (SQLUSMALLINT)index,
				_CTypeConvert[typeid(value).hash_code()], (SQLPOINTER)&value, sizeof(T), cbNum);
		}

		void GetValue(volatile char* buffer, int index, int bufferSize)
		{
			SQLLEN cbNum = SQL_NTS;
			
			SQLRETURN ret = SQLGetData(_stmt, (SQLUSMALLINT)index,
				SQL_C_CHAR, (SQLPOINTER *)buffer, bufferSize, &cbNum);
		}

	private:
		void Timeout(size_t timeout);
	private:
		HSTMT _stmt;
		bool  _open;
		class Connection _conn;

		std::unordered_map<size_t, size_t> _CTypeConvert;
		std::unordered_map<size_t, size_t> _SQLTypeConvert;
	};
}
