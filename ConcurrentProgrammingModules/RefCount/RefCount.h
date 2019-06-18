#pragma once


template <typename Type>
class RefCountPtr
{
private:
	Type* _TypePtr;

public:
	RefCountPtr()
		:_TypePtr(nullptr)
	{}

	explicit RefCountPtr(Type* ptr)
		:_TypePtr(ptr)
	{
		if (_TypePtr)
			_TypePtr->AddRef();
	}

	explicit RefCountPtr(const RefCountPtr& rhs)
		:_TypePtr(rhs._TypePtr)
	{
		if (_TypePtr)
			_TypePtr->AddRef();
	}

	template <typename U>
	explicit RefCountPtr(const RefCountPtr<U>& rhs)
		:_TypePtr(rhs._TypePtr)
	{
		if (_TypePtr)
			_TypePtr->AddRef();
	}

	~RefCountPtr()
	{
		if (_TypePtr)
		{
			_TypePtr->Release();
			_TypePtr = nullptr;
		}
	}

	void Reset()
	{
		RefCountPtr().Swap(*this);
	}

	template <typename U>
	void Reset(U* ptr)
	{
		assert(ptr == nullptr || ptr != _TypePtr);

		RefCountPtr(ptr).Swap(*this);
	}

	void Swap(RefCountPtr& rhs)
	{
		std::swap(_TypePtr, rhs._TypePtr);
	}

	Type* GetRawPtr() const
	{
		return _TypePtr;
	}

	Type* operator->() const
	{
		return _TypePtr;
	}

	Type& operator*() const
	{
		return *_TypePtr;
	}

	bool operator!() const
	{
		return _TypePtr == nullptr;
	}

	RefCountPtr& operator=(const RefCountPtr& rhs)
	{
		RefCountPtr(rhs).Swap(*this);
		return *this;
	}

	template <typename U>
	RefCountPtr& operator=(const RefCountPtr<U>& rhs)
	{
		RefCountPtr(rhs).Swap(*this);
		return *this;
	}
};


template <typename T, typename U>
inline bool operator==(const RefCountPtr<T>& lhs, const RefCountPtr<U>& rhs)
{
	return lhs.GetRawPtr() == rhs.GetRawPtr();
}


template <typename T, typename U>
inline bool operator!=(const RefCountPtr<T>& lhs, const RefCountPtr<U>& rhs)
{
	return lhs.GetRawPtr() != rhs.GetRawPtr();
}