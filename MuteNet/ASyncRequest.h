#pragma once

#include "pch.h"

namespace MuteNet
{
	class ASyncRequest abstract : public OVERLAPPED
	{
	public:
		virtual void Process() = 0;

		virtual void IOCompletion(DWORD TransfferredBytes) = 0;
	};
}

