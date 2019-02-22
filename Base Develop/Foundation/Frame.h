#pragma once

#include "time.h"
#include "DeltaTime.h"


namespace Timing
{
	class Frame final
	{
	public:
		Frame()
			:_frameCount(0), _framerate(0)
		{
			_frameTimer.Start(1000);
		}

		bool Check(Tick tick);
		uint32_t get_framerate() const;
	private:
		uint32_t _frameCount;
		uint32_t _framerate;
		DeltaTime _frameTimer;
	};

	inline bool Frame::Check(Tick tick)
	{
		++_frameCount;

		if (true == _frameTimer.Check(tick))
		{
			_framerate = (uint32_t)((size_t)(_frameCount * 1000) / _frameTimer.ElapsedTick());
			_frameCount = 0;
			return true;
		}
		return false;
	}

	inline uint32_t Frame::get_framerate() const
	{
		return _framerate;
	}
}

