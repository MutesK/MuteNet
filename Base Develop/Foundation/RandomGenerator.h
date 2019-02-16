#pragma once

#include <random>
#include "foundation.h"
#include "Singleton.hpp"
/*
	���� �� ���
	-> mt19937 �� Thread-Safe���� �ʱ� ������, ���� ����ϸ鼭 ¥�ߵ�.
*/
class RandomGenerator : public Singleton<RandomGenerator>
{
public:
	RandomGenerator();
	~RandomGenerator();

	void SetSeed(uint32_t Seed);
	float GetRandomFloat();
	uint32_t GetRandomUInt();
	int32_t GetRandomInt();

private:
	std::mt19937 _generator;
	std::mutex	 _mutex;
};

