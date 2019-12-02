#pragma once

#include "pch.h"
#include "../Foundation/ASyncIOTask.h"

namespace MuteNet
{
	struct ExtentedOverlapped
	{
		OVERLAPPED Overlapped;
		void* SelfPtr;
	};

	class ASyncRequest : public Util::ASyncIOTask
	{
	public:
		ExtentedOverlapped Overlapped;
	protected:
		ASyncRequest() = default;
		virtual ~ASyncRequest() = default;

		virtual bool Process() override
		{
			ZeroMemory(&Overlapped.Overlapped, sizeof(OVERLAPPED));

			return true;
		}

	public:
		virtual void IOCompletion(DWORD TransfferredBytes) override
		{

		}

		virtual void IOError(DWORD Error) override
		{

		}

	};
}

