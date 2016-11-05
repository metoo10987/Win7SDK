#pragma once

class ipc_msg
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 1024 };

	ipc_msg(void);
	~ipc_msg(void);
	const char* data() const;
	char* data();
	size_t length() const;
	const char* body() const;
	char* body();
	size_t body_length() const;
	void body_length(size_t new_length);
	bool decode();
	void encode();
private:
	char data_[header_length + max_body_length];
	size_t body_length_;
};
