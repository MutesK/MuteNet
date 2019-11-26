#pragma once

/*
	Log ���� ��� �̸� ���� & FileStream�� ������ LoggerWriter���� �佺

	���� �뷮�̻�, �ð�
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

