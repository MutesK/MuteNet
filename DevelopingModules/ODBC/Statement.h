#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include <sql.h>
#include <sqlext.h>

class Statement final
{
private:
	SQLHSTMT _stmt = nullptr;
	SQLRETURN _lastReturn;

	NON_COPYABLE(Statement);
public:
	explicit Statement(const SQLHDBC DBCHandle);
	~Statement();

	auto ExecuteQuery(char* query) -> SQLRETURN;
	auto PrepareQuery(char* query)->SQLRETURN;

	auto Fetch() -> SQLRETURN;

	auto GetData(short int& OUT value, int ColumnNum) -> void;
	auto GetData(int& OUT value, int ColumnNum) -> void;
	auto GetData(float& OUT value, int ColumnNum) -> void;
	auto GetData(double& OUT value, int ColumnNum) -> void;
	auto GetData(std::string& OUT value, int ColumnNum) -> void;

	auto BindParameter(int ColumnNum, const short int& IN value) -> SQLRETURN;
	auto BindParameter(int ColumnNum, const int& IN value)->SQLRETURN;
	auto BindParameter(int ColumnNum, const float& IN value)->SQLRETURN;
	auto BindParameter(int ColumnNum, const double& IN value)->SQLRETURN;
	auto BindParameter(int ColumnNum, const std::string& IN value)->SQLRETURN;

};
