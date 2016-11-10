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
		////////////���� acces_traits
		template< typename T>
		struct access_traits
		{
			typedef const T& const_type;
			typedef T& non_const_type;
			typedef const typename const_cast<T>::type& parameter_type;
		};

		/////////////���� element
		template< int N, typename T>
		struct element
		{
		private:
			typedef typename T::tail_type next;
		public:
			typedef typename element<N-1, next>::type type;
		};

		////////////���� element ��ƫ�ػ�
		template<typename T>
		struct element<0,T>
		{
			typedef typename T::header_type type;
		};

		////////////���� cons
		template<typename T0, typename T1>
		struct cons
		{
			typedef T0 head_type;		// �û��ṩ����������
			typedef T1 tail_type;			// ͨ����һ��cons<>�ľ�����
			head_type head;				//�����һ�����ݳ�Ա
			tail_type tail;						//����ڶ������ݳ�Ա
		};

		///////////���� cons ��ƫ�ػ�
		template<typename T0>
		struct cons<T0, null_type>
		{
			typedef T0 head_type;
			typedef null_type tail_type;
			typedef cons<T0, null_type> self_type;
			head_type head;
		};

		////////////���� map_tuple_to_cons
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

		////////////���� map_tuple_to_cons���ػ�
		template<>
		struct map_tuple_to_cons< null_type, null_type, null_type,
												   null_type, null_type, null_type,
												   null_type, null_type, null_type,
												   null_type >
		{
			typedef null_type type;
		};

		///////////���� get_class
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
