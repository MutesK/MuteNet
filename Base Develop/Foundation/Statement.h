#pragma once

/*
	Bind Parameter, 
	Prepare, 
	SQLCancle,
	QueryTimeout
	DirectExecute
	Execute
	RowCount
	ResetParameter
	ParameterCount
	ParameterSize
*/
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
				_CTypeConvert[typeid(value)], _SQLTypeConvert[typeid(value)],
				0, 0, &value, 0, &SQL_NTS);
			
			if (rc != SQL_SUCCESS)
			{
				return false;
			}

			return true;
		}

	private:
		void Timeout(long timeout);
	private:
		HSTMT _stmt;
		bool  _open;
		class Connection _conn;

		std::unordered_map<std::type_info, SQLSMALLINT> _CTypeConvert;
		std::unordered_map<std::type_info, SQLSMALLINT> _SQLTypeConvert;
	};
}
