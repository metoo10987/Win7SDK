#include "io_server.h"
using namespace services;

io_server::io_server(void):m_write_index(0), m_read_index(0)
{
	InitializeCriticalSection(&m_r_loop_message_queue_cs);
}

io_server::~io_server(void)
{
}
