#include "RandomGenerator.h"



RandomGenerator::RandomGenerator()
{
	_seed = -1;
}


RandomGenerator::~RandomGenerator()
{
}

void RandomGenerator::SetSeed()
{
	_seed = static_cast<uint32_t>(std::hash<std::thread::id>()(std::this_thread::get_id()));
}

uint32_t RandomGenerator::GetRandomUInt(uint32_t min, uint32_t max)
{
	SetSeed();

	static thread_local std::mt19937 generator(_seed);
	std::uniform_int_distribution<uint32_t> distribution(min, max);
	return distribution(generator);
}


int32_t RandomGenerator::GetRandomInt(int32_t min, int32_t max)
{
	SetSeed();

	static thread_local std::mt19937 generator(_seed);
	std::uniform_int_distribution<int32_t> distribution(min, max);
	return distribution(generator);
}


float RandomGenerator::GetRandomFloat(float min, float max)
{
	SetSeed();

	static thread_local std::mt19937 generator(_seed);
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}