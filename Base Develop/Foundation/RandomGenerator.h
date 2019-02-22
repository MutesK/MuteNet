#pragma once

#include <random>
#include "foundation.h"
#include "Singleton.hpp"
/*
	·£´ý °ª Ãâ·Â
	-> Random Device isn't Thread Safe
*/
class RandomGenerator : public Singleton<RandomGenerator>
{
public:
	RandomGenerator();
	~RandomGenerator();

	uint32_t GetRandomUInt(uint32_t min, uint32_t max);
	int32_t GetRandomInt(int32_t min, int32_t max);
	float GetRandomFloat(float min, float max);
private:
	void SetSeed();
private:
	uint32_t		    _seed;
};

