#pragma once

#include <gtest/gtest.h>


class SocketTest : public ::testing::Test
{
protected:
	void SetUp() override;

	void TearDown() override;
};

