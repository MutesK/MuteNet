#pragma once

#include <gtest/gtest.h>

class NetworkTest : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;
};

