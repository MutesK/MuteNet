#pragma once

/*
	Log 파일 출력 이름 정의 & FileStream을 가지고 LoggerWriter한테 토스

	일정 용량이상, 시간
	OutputDirectoryFolder/YYYY-MM-DD/Name_YYYY-MM-DD_Number.log
*/

#include "Logger.h"

class RotateLogger final
{
private:
	std::string _appName;
	std::string _directory;
	size_t		_logFileNum = 0;

	std::string _LogFileName;
public:
	void Initialize(const std::string& AppName,
		const std::string& OutputDirectoryFolder);

	std::string GetFullPath();
	void NextLogNumber();
private:
	void SetFileNumber();
};

