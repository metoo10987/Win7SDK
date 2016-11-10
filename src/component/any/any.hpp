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
		//提供关于型别的信息
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
			//typeid返回std::typeinfo对象引用，std::typeinfo对象包含任意对象
			//的型别信息如name，还提供==操作符
			//可以使用typeid(oneObj) == typeid(anotherObj)来比较两个对象间型别是否一致
			return typeid(ValueType);
		}

		virtual placeholder * clone() const{
			return new holder(held);
		}

	public:
		ValueType held;		//保存数据
	};
	placeholder * content;	//指向泛型数据容器holder的基类placeholder的指针

	any()
		: content(NULL)
	{}

	template<typename ValueType>
	any(const ValueType & value)
		: content(new holder<ValueType>(value))
	{}

	template<typename ValueType>
	any & operator = (const ValueType & rhs){
		//交换两个指针的值，由于any(rhs)是一个临时变量，所以函数退出后自动析构原来的数据
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
