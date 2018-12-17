#pragma once

#include "foundation.h"

class PathImpl
{
public:
	// 현재 Path 가져온다.
	static std::string current();
	// 현재 실행중인 프로그램의 실행경로 가져옴
	static std::string self();
	// PC 정보(환경변수) 가져온다.
	static std::string expand(const std::string& path);
	// 루트 드라이버 출력
	// ex) C:, D: 
	static void listDriver(std::vector<std::string>& OUT roots);
};

