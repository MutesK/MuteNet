#pragma once

#include "foundation.h"

class PathImpl
{
public:
	// ���� Path �����´�.
	static std::string current();
	// ���� �������� ���α׷��� ������ ������
	static std::string self();
	// PC ����(ȯ�溯��) �����´�.
	static std::string expand(const std::string& path);
	// ��Ʈ ����̹� ���
	// ex) C:, D: 
	static void listDriver(std::vector<std::string>& OUT roots);
};

