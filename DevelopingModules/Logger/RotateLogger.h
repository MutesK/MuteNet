#pragma once

/*
	Log 파일 출력 이름 정의

	일정 용량이상, 시간
	OutputDirectoryFolder/YYYY-MM-DD/Name_YYYY-MM-DD_Number.log
*/

#include "Logger.h"

class RotateLogger final
{
private:
	std::string _name;
	std::string _directory;

	size_t		_LogFileNum;
public:
	RotateLogger() = default;
	~RotateLogger() = default;

	void Initialize(const std::string& AppName,
		const std::string& OutputDirectoryFolder);

	std::string GetOutputLogFolder();

private:
	void SetFileNumber();
	std::string GetCurrentDate();
};

