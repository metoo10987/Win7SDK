#include <string>
using namespace std;

template <typename T>
struct separater
{
	separater(const string& str, T separate_flag){}
};

template <>
struct separater<int>
{
	separater(const string& str, int step_size)
	{
		p_pos = str.c_str();
		surplus_len = str.size();
		this->step_size = step_size;
	}

	bool get_next(const char** desc, int& len)
	{
		if(surplus_len > step_size)
		{
			*desc = p_pos;
			len = step_size;
			surplus_len -= step_size;
			p_pos += step_size;
			return true;
		}else
		{
			*desc = p_pos;
			len = surplus_len;
			surplus_len = 0;
			p_pos = NULL;
			return false;
		}
	}

	int step_size;				//步长
	const char* p_pos;	//当前位置
	int surplus_len;			//字符串剩余长度
};

template<>
struct separater<char>
{
	separater(const string& str, char c)
	{
		this->str = str;
		pos = this->str.find(c);
		p_pos = this->str.c_str();
		last_pos = 0;
		this->c = c;
		p_begin = str.c_str();
	}
	bool get_next(const char** desc, int& len)
	{
		if(pos != string::npos)
		{
			//计算长度，获取指针位置
			len = pos - last_pos;
			*desc = p_pos;

			//移到下一个开始位置,并记录上一个位置记录
			p_pos = p_begin+pos+1;
			pos += 1;
			last_pos = pos;

			//从新的位置开始查找
			pos = str.find(c,pos);
			return true;
		}else
		{
			*desc = p_pos;
			len = str.size() - last_pos;
			return false;
		}
	}
	string str;
	int pos;
	int last_pos;
	const char* p_pos;
	const char* p_begin;
	char c;
};

template <typename T>
struct string_separater
{
	string_separater(const string& str, T flag) : separater_instance(str,flag){}
	separater<T> separater_instance;
	bool get_next_string(string& desc)
	{
		desc = "";
		int len = 0;
		const char* p = NULL;
		bool no_end = true;
		no_end = separater_instance.get_next(&p,len);
		desc.append(p,len);
		return no_end;
	}
};