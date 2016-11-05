#ifndef _XFILE_API_H
#define _XFILE_API_H
#include <string>
#include <WTypes.h>
using namespace std;
#include "../macro/config.h"

namespace xfile_helper
{
	XFILE_API BOOL set_folder_icon(const string& path, const string& iconSource, int index = 0);

	XFILE_API BOOL set_folder_show_name(const string& path, const string& name);
	XFILE_API BOOL set_folder_show_name(const string& path, const string& resource, int index);
	
	XFILE_API BOOL set_folder_infotip(const string& path, const string infotip);
	XFILE_API BOOL set_folder_infotip(const string& path, const string& resource, int index);

	XFILE_API void notify_changed(const string& path);
}
#endif