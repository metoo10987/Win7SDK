#include "context.h"


namespace xfile
{
	void init_context(HMODULE hModule)
	{
		char module_name[MAX_PATH];
		GetModuleFileNameA(hModule, module_name, MAX_PATH);
		module_path = module_name;
	}
	string module_path;
}