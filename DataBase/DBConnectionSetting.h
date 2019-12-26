#pragma once

struct DBConnectionSetting final
{
	std::string _DBName;
	uint32_t _DBNameIndex;
	std::string _Host;
	std::string _Port;
	std::string _User;
	std::string _Passwd;
	std::string _db;
};

