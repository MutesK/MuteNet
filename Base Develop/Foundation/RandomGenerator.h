#pragma once

#include <random>
#include "foundation.h"
#include "Singleton.hpp"
/*
	랜덤 값 출력
	-> mt19937 가 Thread-Safe하지 않기 때문에, 이점 고려하면서 짜야됨.
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

