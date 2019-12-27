#pragma once

struct DBConnectionSetting final
{
	std::string _DBName;
	uint8_t _DBNameIndex;
	std::string _Host;
	uint16_t _Port;
	std::string _User;
	std::string _Passwd;
	std::string _db;
};

