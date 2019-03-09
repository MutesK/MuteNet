#pragma once

#include "../Foundation/ObjectPoolTLS.h"

template <class Function>
class WorkItem : public CObjectPoolTLS<WorkItem<Function>>
{
	static_assert(!std::is_function(Function)::value, "WorkItem's Template Type have to set to Function Type");
public:
	WorkItem(Function&& f)
		:_function(std::move(f))
	{
	}

	WorkItem(const WorkItem&& item)
	{
		_function = std::move(item._function);
	}

	~WorkItem()
	{}

	template <typename... Args>
	void Process(Args ...args)
	{
		_function(std::forward<Args>(args)...);
	}

private:
	Function _function;
};