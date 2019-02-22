#pragma once

#include "time.h"

namespace Timing
{
	class DeltaTime final
	{
	public:
		DeltaTime() :
		_running(false), _interval(0), _currentTick(0), _elapsedTick(0), _lastTick(0)
		{}
		void Start(Tick interval, Tick StartTick = 0);
		void Stop();
		void Update(Tick CurrentTick);
		bool Check(Tick CurrentTick);
		bool isRunning() const;
		Tick interval() const;
		Tick currentTick() const;
		Tick ElapsedTick() const;
	private:
		bool _running;
		Tick  _interval;
		Tick  _currentTick;
		Tick  _elapsedTick;
		Tick  _lastTick;
	};

	inline void DeltaTime::Start(Tick interval, Tick StartTick)
	{
		_interval = interval;
		_currentTick = StartTick;
		_elapsedTick = 0;
		_lastTick = StartTick;
		_running = true;
	}

	inline void DeltaTime::Stop()
	{
		_running = false;
		_interval = 0;
		_currentTick = 0;
		_elapsedTick = 0;
		_lastTick = 0;
	}

	inline void DeltaTime::Update(Tick CurrentTick)
	{
		if (_running)
			_currentTick = CurrentTick;
	}

	inline bool DeltaTime::Check(Tick CurrentTick)
	{
		if (!_running)
			return false;

		_currentTick = CurrentTick;
		_elapsedTick = _currentTick - _lastTick;

		if (_elapsedTick >= _interval)
		{
			_lastTick = _currentTick;
			return true;
		}

		return false;
	}

	inline bool DeltaTime::isRunning() const
	{
		return _running;
	}

	inline Tick DeltaTime::interval() const
	{
		return _interval;
	}

	inline Tick DeltaTime::currentTick() const
	{
		return _currentTick;
	}

	inline Tick DeltaTime::ElapsedTick() const
	{
		return _elapsedTick;
	}

}