#include "ipc_msg.h"

#include<string>
using namespace std;

ipc_msg::ipc_msg(void): body_length_(0)
{
}

ipc_msg::~ipc_msg(void)
{
}

const char* ipc_msg::data() const
{
	return data_;
}

char* ipc_msg::data()
{
	return data_;
}

size_t ipc_msg::length() const
{
	return header_length + body_length_;
}

const char* ipc_msg::body() const
{
	return data_ + header_length;
}

char* ipc_msg::body()
{
	return data_ + header_length;
}

size_t ipc_msg::body_length() const
{
	return body_length_;
}

void ipc_msg::body_length( size_t new_length )
{
	body_length_ = new_length;
	if (body_length_ > max_body_length)
		body_length_ = max_body_length;
}

bool ipc_msg::decode()
{
	char header[header_length + 1] = "";
	strncat(header, data_, header_length);
	body_length_ = atoi(header);
	if (body_length_ > max_body_length)
	{
		body_length_ = 0;
		return false;
	}
	data_[header_length+body_length_] = '\0';
	return true;
}

void ipc_msg::encode()
{
	char header[header_length + 1] = "";
	sprintf(header, "%4d", body_length_);
	memcpy(data_, header, header_length);
}
