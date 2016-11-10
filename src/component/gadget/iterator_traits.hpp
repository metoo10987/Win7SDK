#ifndef ITERATOR_TRAITS_INCLUDE
#define ITERATOR_TRAITS_INCLUDE

template <typename T>
struct iterator_traits
{	// get traits from iterator T
	typedef typename T::value_type value_type;
	typedef typename T::reference reference;
	typedef typename T::pointer pointer;
	typedef typename difference_type distance_type;	// retained
	typedef typename T::iterator_category iterator_category;
};

template <typename T>
struct iterator_traits<T *>
{	// get traits from pointer
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef ptrdiff_t distance_type;// retained
	typedef T * pointer;
	typedef T& reference;
};

template<typename T>
struct iterator_tratis<const T *>
{	// get traits from const pointer
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef ptrdiff_t distance_type;// retained
	typedef const T *pointer;
	typedef const T& reference;
};

#endif //ITERATOR_TRAITS_INCLUDE