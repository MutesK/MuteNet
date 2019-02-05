#pragma once

/*
	� ����� Getter, Setter�� OOP�� ���ʶ߸��� ���̶�� ������,
	
	OOP�� Object���� ���� ��Ű�°� ��Ģ�̰�, public���� �ش� �Ӽ��� �ٷ� �����ϴ� ���� �����Ҽ��� ����. (������ ������� �ִ�.)
	��ũ�� �Լ��� ���� �ʰ�, �ִ��� ���ø����� �����Ѵ�. (C++���� Template ��Ÿ�� ���������� ��ũ�� �Լ��� ���輺�� ���� -> �ٷ� Overwrtie�ϱ� ����)
	
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

	// ����� �Լ�
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
