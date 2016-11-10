#pragma once
#include <exception>
using namespace std;
class bad_except :
	public exception
{
public:
	bad_except(void);
	~bad_except(void);
	int m_error_code;
};

