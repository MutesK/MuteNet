#pragma once

#include <gtest/gtest.h>
#include <Socket.h>
#include <IODispatcher.h>

using namespace MuteNet;

class DispatcherTest : public ::testing::Test
{
protected:
	IODispatcher dispatcher;
	Socket serverSock;
	Socket clientSock;
	Socket acceptedSock;

	const std::string ip = "127.0.0.1";
	uint16_t port = 0;

	void SetUp() override;

	void TearDown() override;
};

