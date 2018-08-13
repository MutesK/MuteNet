#pragma once

class Event
{
public:
	Event();
	~Event();

	void SetEventName(const std::string& IN _eventName);
	void SetResetFlag(bool resetFlag);

	void CreateEvent();
	bool SetEvent();
	bool ResetEvent();
	bool PulseEvent();
	
	void WaitSignal();
private:
	HANDLE				_event;
	bool				_resultflag;

	std::string			_eventName;
};
inline void Event::CreateEvent()
{
	_event = ::CreateEvent(nullptr, _resultflag, true, _eventName.c_str());
}

inline void Event::SetResetFlag(bool resetFlag)
{
	_resultflag = resetFlag;
}

inline void Event::SetEventName(const std::string& IN eventName)
{
	_eventName = eventName;
}

inline bool Event::SetEvent()
{
	bool ret = ::SetEvent(_event) != 0;
	return ret;
}
inline bool Event::ResetEvent()
{
	bool ret = ::ResetEvent(_event) != 0;
	return ret;
}
inline bool Event::PulseEvent()
{
	bool ret = ::PulseEvent(_event) != 0;
	return ret;
}