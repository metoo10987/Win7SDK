#define IO_SERVICE_API extern "C" _declspec(dllexport)
#define LOOP_MESSAGE_QUEUE_MAX_NUM 100000
#include "io_service.h"
#include <io/io_server.h>

services::io_server * p_io_server;

IO_SERVICE_API void init_io_serviece( int thread_count )
{
	p_io_server = new services::io_server();
	p_io_server->run(thread_count);
}

IO_SERVICE_API bool  post_commond( std::function<void(std::string)>* cmd, std::string param )
{
	return p_io_server->post_cmd(cmd,param);
}

