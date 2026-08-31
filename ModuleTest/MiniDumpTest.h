#pragma once

#include "Common.h"

namespace fs = std::filesystem;

class MiniDumpTest : public ::testing::Test
{
protected:
	fs::path testDumpDir;

	void SetUp() override;

	void TearDown() override;

	bool HasDumpFileBeenCreated() const;
};

