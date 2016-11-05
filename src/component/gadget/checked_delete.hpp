#ifndef CHECKED_DELETE_INCLUDE
#define CHECKED_DELETE_INCLUDE

template<typename T>
inline void checked_delete(T * x)
{
	typedef char type_must_be_complete[sizeof(T)? 1: -1];
	(void) sizeof(type_must_be_complete);
	delete x;
}

template<typename T>
inline void checked_array_delete(T * x)
{
	typedef char type_must_be_complete[sizeof(T)? 1: -1];
	(void) sizeof(type_must_be_complete);
	delete [] x;
}

template<typename T>
struct checked_deleter
{
	typedef void result_type;
	typedef T * argument_type;
	void operator()(T * x) const
	{
		checked_delete(x);
	}
};

template<typename T>
struct checked_array_deleter
{
	typedef void result_type;
	typedef T * argument_type;
	void operator()(T * x) const
	{
		checked_arrar_delete(x);
	}
};

#endif	//CHECKED_DELETE_INCLUDE