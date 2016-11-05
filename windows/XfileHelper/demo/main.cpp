
#include "../XfileHelper/src/XfileHelper/folder_helper.h"

int main()
{
	string path = "D:/123";
	string iconSource = "D:/demo/Xfile/Xfile/src/resource/icon/100.ico";

	xfile_helper::set_folder_icon(path, iconSource.c_str());
	xfile_helper::set_folder_show_name(path, "999");
	xfile_helper::set_folder_infotip(path,"000");
	return 0;
}