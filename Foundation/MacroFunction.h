#pragma once


#define NON_COPYABLE(name)					\
name(const name&) = delete;					\
name(const name&&) = delete;				\
name& operator=(const name&) = delete;		\
name& operator=(const name&&) = delete;

#define GET_SET_ATTRIBUTE(TYPE, VALUE) \
	TYPE get_##VALUE()				   \
	{								   \
		return _##VALUE;			   \
	}								   \
	void set_##VALUE(##TYPE value)     \
	{								   \
		_##VALUE = value;		       \
	}

#define GET_CONST_ATTRIBUTE(TYPE, VALUE) \
	TYPE get_##VALUE() const	       \
	{								   \
		return _##VALUE;			   \
	}	

#define IN
#define OUT

