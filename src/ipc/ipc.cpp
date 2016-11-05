#include <ipc/ipc_impl.h>

#define IPC_API extern "C" __declspec(dllexport)

#include "ipc.h"
ipc_impl * p_ipc;
ipc_impl ipc;

IPC_API void init_ipc()
{
	p_ipc = new ipc_impl();
}

IPC_API void to_connect(string name)
{
	p_ipc->connect(name);
}

IPC_API void register_read_handler( handle_type handler )
{
	p_ipc->register_listen_handler(handler);
}

IPC_API void to_send( string name, const char* p_src, int langth )
{
	p_ipc->write(name,p_src, langth);
}
