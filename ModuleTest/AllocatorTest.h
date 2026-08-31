#pragma once

#include <gtest/gtest.h>

class AllocatorTest : public ::testing::Test
{
protected:
	void SetUp() override;

	void TearDown() override;
};