#ifndef IPC_INCLUDE
#define IPC_INCLUDE

#include <functional>
#include <string>
using namespace std;
#ifdef IPC_API
#else
#define IPC_API extern "C" __declspec(dllimport)
#endif	//	IPC_API

typedef std::tr1::function<void(char* , int)> handle_type;

// init_ipc 初始化 IPC 应最先调用该函数
IPC_API void init_ipc();

// 监听一个缓冲区，参数name是缓冲名字
// 当调用to_send函数向这个名字的缓冲区发送数据时，会自动接收到数据
// 并调用通过register_read_handler注册的处理函数
IPC_API void start_listen(string name);

//	连接到一个缓冲区name要连接到的缓冲区的名字，
//	连接后可以通过to_send向这个缓冲区写数据
IPC_API void to_connect(string name);

//	向一个名为name的缓冲区发送数据，在调用该函数前这个缓冲区必须已经通过to_connect连接了
//	p_str指向一个缓冲区，这个缓冲区中的内容将被写入到name命名的缓冲区中
//	langth写入name缓冲区中数据的长度
IPC_API void to_send(string name, const char* p_src, int langth);

//	注册一个数据处理函数，当被监听的缓冲区有数据写入时，会自动调用被注册的函数
IPC_API void register_read_handler(handle_type handler);

#endif	// IPC_INCLUDE

