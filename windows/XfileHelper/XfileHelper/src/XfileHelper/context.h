#ifndef _CONTEX_H
#define _CONTEX_H
#include <string>
using namespace std;

#include <Windows.h>

namespace xfile
{
	extern string module_path;

	void init_context(HMODULE hModule);
}
#endif