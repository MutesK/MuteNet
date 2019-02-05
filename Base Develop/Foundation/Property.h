#pragma once

/*
	어떤 사람은 Getter, Setter가 OOP를 무너뜨리는 것이라고 보지만,
	
	OOP는 Object에게 일을 시키는게 원칙이고, public에서 해당 속성을 바로 조작하는 것은 위험할수도 있음. (추적이 힘들수가 있다.)
	매크로 함수를 쓰지 않고, 최대한 템플릿으로 설계한다. (C++에서 Template 메타가 나온이유가 매크로 함수에 위험성이 있음 -> 바로 Overwrtie하기 때문)
	
*/

template <typename Type>
class get
{
private:
	get(const Type& data) :
		_data(data)
	{}
	get(const get<Type>& get_data) :
		_data(get_data._data)
	{}

	operator const Type& () const // get()
	{
		return _data;
	}

	const Type& operator=(const Type& data)
	{
		// Junk
	}

private:
	get<Type>& operator=(const get<Type>&)
	{
		// Junk
	}
public:
	const Type& _data;
};

template <typename Type>
class set
{
public:
	set(Type& data) : _data(data)
	{}
	set(const set<Type>& set_data) : _data(set_data._data)
	{}
	const Type& operator=(const Type& data)
	{
		_data = data;
		return _data;
	}
	operator const Type&() const
	{
		// Junk
	}
private:
	set<Type>& operator=(const set<Type>&)
	{
		// Junk
	}
private:
	Type& _data;
};

template <typename Type>
class all
{
public:
	all(Type& data) : _get(data), _set(data)
	{}

	operator const Type& () const
	{
		return (_get.operator const Type&()); 
	}

	const Type& operator=(const Type& data)
	{
		return _set.operator=(data);
	}
private:
	get<Type> _get;
	set<Type> _set;
};

template <class Type, template<class> class PropertyPolicy = all>
class property : public PropertyPolicy<Type>
{
public:
	property(Type& data) : PropertyPolicy<Type>(data)
	{}

	// 명시적 함수
	const Type& get() const
	{
		return PropertyPolicy<Type>::operator const Type&();
	}

	void set(const Type& data)
	{
		return PropertyPolicy<Type>::operator=(data);
	}
private:
	property(const property<Type, PropertyPolicy>&);
	property& operator=(const property<Type, PropertyPolicy>&);
};
