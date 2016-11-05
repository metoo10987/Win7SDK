#ifndef IO_SERVICE_H
#define IO_SERVICE_H

#include <functional>
#include <string>

#ifndef IO_SERVICE_API
#define IO_SERVICE_API extern "C" _declspec(dllimport)
#endif	//IO_SERVICE_API

IO_SERVICE_API void init_io_serviece(int thread_count = 2);
IO_SERVICE_API bool post_commond(std::function<void(std::string)>* cmd, std::string param);

#endif	//IO_SERVICE_H