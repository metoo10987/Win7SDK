namespace mlb
{
	struct null_type;
	template< typename T0 = null_type, typename T1 = null_type, typename T2 = null_type,
					 typename T3 = null_type, typename T4 = null_type, typename T5 = null_type,
					 typename T6 = null_type, typename T7 = null_type, typename T8 = null_type,
					 typename T9 = null_type >
	class tuple;

	template <int N, typename T0, typename T1>
	inline type detail::access_traits< 
		typename element<N, const<T0,T1>>::type>::non_const_type
		get(detail::cons<T0,T1>& c)
	{
		return detail::get_class<N>::template get<
			typename detail::access_traits<
			typename element<N,cons<T0,T1>>::type>
			::non_const_type>(c)
	}

	namespace detail
	{
		////////////定义 acces_traits
		template< typename T>
		struct access_traits
		{
			typedef const T& const_type;
			typedef T& non_const_type;
			typedef const typename const_cast<T>::type& parameter_type;
		};

		/////////////定义 element
		template< int N, typename T>
		struct element
		{
		private:
			typedef typename T::tail_type next;
		public:
			typedef typename element<N-1, next>::type type;
		};

		////////////定义 element 的偏特化
		template<typename T>
		struct element<0,T>
		{
			typedef typename T::header_type type;
		};

		////////////定义 cons
		template<typename T0, typename T1>
		struct cons
		{
			typedef T0 head_type;		// 用户提供的数据类型
			typedef T1 tail_type;			// 通常是一个cons<>的具现体
			head_type head;				//定义第一个数据成员
			tail_type tail;						//定义第二个数据成员
		};

		///////////定义 cons 的偏特化
		template<typename T0>
		struct cons<T0, null_type>
		{
			typedef T0 head_type;
			typedef null_type tail_type;
			typedef cons<T0, null_type> self_type;
			head_type head;
		};

		////////////定义 map_tuple_to_cons
		template< typename T0, typename T1, typename T2,
						 typename T3, typename T4, typename T5,
						 typename T6, typename T7, typename T8,
						 typename T9 >
		struct map_tuple_to_cons
		{
			typedef cons<T0,
				typename map_tuple_to_cons<
					T1, T2, T3, T4, T5, T6, T7, T8, T9, null_type>::type
			> type;
		};

		////////////定义 map_tuple_to_cons的特化
		template<>
		struct map_tuple_to_cons< null_type, null_type, null_type,
												   null_type, null_type, null_type,
												   null_type, null_type, null_type,
												   null_type >
		{
			typedef null_type type;
		};

		///////////定义 get_class
		template<int N>
		struct get_class
		{
			template<typename RET, typename T0, typename T1>
			inline static RET get(cons<T0, T1>& t)
			{
				return get_class<N-1>::template get<RET>(t.tail);
			}
		};

		template<>
		struct get_class<0>
		{
			template<typename RET, typename T0, typename T1>
			inline static RET get(cons<T0,T1>& t)
			{
				return t.head;
			}
		};
	}
}
