#pragma once

#include "pch.h"

namespace MuteNet
{
	struct ExtentedOverlapped
	{
		OVERLAPPED Overlapped;
		void* SelfPtr;
	};

	class ASyncRequest abstract
	{
	public:
		ExtentedOverlapped Overlapped;
	protected:
		ASyncRequest()
			:Overlapped{0}
		{
		}
		virtual ~ASyncRequest()
		{
		}

		virtual bool Process()
		{
			ZeroMemory(&Overlapped.Overlapped, sizeof(OVERLAPPED));

			return true;
		}
	public:
		virtual void IOCompletion(DWORD TransfferredBytes) = 0;

		virtual void IOError(DWORD Error) = 0;

	};
}

