#pragma once

/*
	Log 파일 출력 이름 정의

	일정 용량이상, 시간
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

