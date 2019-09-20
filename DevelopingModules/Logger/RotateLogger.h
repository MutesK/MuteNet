#pragma once

/*
	Log ���� ��� �̸� ����

	���� �뷮�̻�, �ð�
	OutputDirectoryFolder/YYYY-MM-DD/Name_YYYY-MM-DD_Number.log
*/

#include "Logger.h"

class RotateLogger final
{
private:
	std::string _name;
	std::string _directory;

	size_t		_fileNum;
public:
	RotateLogger(const std::string& AppName, 
		const std::string& OutputDirectoryFolder);

	std::string GetOutputLogFolder();

private:
	void SetFileNumber();
	std::string GetCurrentDate();
};

