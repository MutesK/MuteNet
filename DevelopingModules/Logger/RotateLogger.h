#pragma once

/*
	Log ���� ��� �̸� ����

	���� �뷮�̻�, �ð�
	/YYYY-MM-DD/Name_YYYY-MM-DD_Number.log
*/

#include "Logger.h"

class RotateLogger final
{
private:
	std::string _fileName;
	size_t		_fileNum;
public:
	std::string GetOutputLogFolder();
};

