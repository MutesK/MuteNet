#pragma once

#include "pch.h"

namespace Util
{
	__interface ASyncIOTask
	{
		virtual bool Process() = 0;

		virtual void IOCompletion(DWORD TransfferredBytes) = 0;
		virtual void IOError(DWORD Error) = 0;
	};
}