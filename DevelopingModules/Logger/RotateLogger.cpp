#include "RotateLogger.h"
#include "StringFormat.h"

using namespace std::experimental::filesystem::v1;

void RotateLogger::Initialize(const std::string& AppName, const std::string& OutputDirectoryFolder)
{
	_appName = AppName;
	_directory = OutputDirectoryFolder;
	_logFileNum = 0;

	SetFileNumber();
}

std::string RotateLogger::GetFullPath()
{
	SetFileNumber();
	return _directory + "//" + _LogFileName;
}

void RotateLogger::NextLogNumber()
{
	++_logFileNum;
}

void RotateLogger::SetFileNumber()
{
	_LogFileName = StringFormat::SharpFormat("{0}_{1}_{2}.log", _appName, StringFormat::GetCurrentDate(), _logFileNum);
}
