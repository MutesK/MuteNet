#include "RotateLogger.h"
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <sstream>

using namespace std::experimental::filesystem::v1;

RotateLogger::RotateLogger(const std::string& AppName,
	const std::string& OutputDirectoryFolder)
{
	_fileNum = 0;

	_name = AppName;
	_directory = OutputDirectoryFolder;

	SetFileNumber();
}

std::string RotateLogger::GetOutputLogFolder()
{
	
}

void RotateLogger::SetFileNumber()
{
	std::string folder =
		_directory + "//" + GetCurrentDate();


}


std::string RotateLogger::GetCurrentDate()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
	return ss.str();
}