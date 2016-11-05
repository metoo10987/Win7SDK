#ifndef ANY_INCLUDE
#define ANY_INCLUDE

#include <typeinfo>
#include <algorithm>
using namespace std;

class any{
public:

	class placeholder{
	public:
		virtual ~placeholder(){}
	public:
		//�ṩ�����ͱ����Ϣ
		virtual const std::type_info & type() const = 0;
		virtual placeholder * clone() const = 0;
	};

	template<typename ValueType>
	class holder : public placeholder{
	public:
		holder(const ValueType & value)
			: held(value){}
	public:
		virtual const std::type_info & type() const{
			//typeid����std::typeinfo�������ã�std::typeinfo��������������
			//���ͱ���Ϣ��name�����ṩ==������
			//����ʹ��typeid(oneObj) == typeid(anotherObj)���Ƚ�����������ͱ��Ƿ�һ��
			return typeid(ValueType);
		}

		virtual placeholder * clone() const{
			return new holder(held);
		}

	public:
		ValueType held;		//��������
	};
	placeholder * content;	//ָ������������holder�Ļ���placeholder��ָ��

	any()
		: content(NULL)
	{}

	template<typename ValueType>
	any(const ValueType & value)
		: content(new holder<ValueType>(value))
	{}

	template<typename ValueType>
	any & operator = (const ValueType & rhs){
		//��������ָ���ֵ������any(rhs)��һ����ʱ���������Ժ����˳����Զ�����ԭ��������
		any(rhs).swap(*this);
		return *this;
	}

	any & operator = (const any& rhs)
	{
		any().swap(*this);
		this->content = rhs.content->clone();
		return *this;
	}

	any & swap(any & rhs){
		std::swap(content,rhs.content);
		return *this;
	}

	const std::type_info & type() const{
		return content ? content->type() : typeid(void);
	}

	~any(){
		delete content;
	}
};

template<typename ValueType>
inline ValueType * any_cast(any * operand){
	return operand && operand->type() == typeid(ValueType) ? 
		&static_cast<any::holder<ValueType>* >(operand->content)->held:0;
}

template<typename ValueType>
inline ValueType * any_cast(const any * operand){
	return any_cast<ValueType>(const_cast<any*>(operand));
}

template<typename ValueType>
inline ValueType any_cast(const any & operand){
	ValueType * result = any_cast<ValueType>(&operand);
	if(!result)
		throw "bad_any_cast";
	return *result;
}

#endif //ANY_INCLUDE
