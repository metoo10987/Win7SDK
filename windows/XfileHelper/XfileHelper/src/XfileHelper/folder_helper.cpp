#include "folder_helper.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include "../macro/error_check.h"

using namespace std;

#pragma comment(lib, "shlwapi.lib")
namespace xfile_helper
{
#define  SECTION_NAME  ".ShellClassInfo"
#define  CONFIT_FILE		"Desktop.ini"
	extern string module_path;

	string build_value(const string resource, int index)
	{
		char strIndex[32];
		sprintf(strIndex, "%d", index);
		return resource + "," + string(strIndex);
	}

	BOOL write_ini(const string& iniFile, const string& section, const string& key, const string& val)
	{
		return WritePrivateProfileStringA(section.c_str(), key.c_str(), val.c_str(), iniFile.c_str());
	}

	BOOL write_ini(const wstring& iniFile, const wstring& section, const wstring& key, const wstring& val)
	{
		return WritePrivateProfileStringW(section.c_str(), key.c_str(), val.c_str(), iniFile.c_str());
	}

	BOOL do_change(const string& folderPath, const string& iniFile)
	{
		BOOL ret = TRUE;
		ret = SetFileAttributesA(iniFile.c_str(), FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
		ERROR_CHECK(ret, TRUE);
		ret = SetFileAttributesA(folderPath.c_str(), FILE_ATTRIBUTE_SYSTEM);
		return ret;
	}

	XFILE_API BOOL set_folder_icon( const string& path, const string& iconSource, int index)
	{
		BOOL ret = TRUE;
		const string iniFile = path + "/" + CONFIT_FILE;
		//∑¿÷π…±∂æ»Ìº˛…®√Ë
		HANDLE hFile = CreateFileA(iniFile.c_str(),
			GENERIC_WRITE | GENERIC_READ, 
			FILE_SHARE_WRITE | FILE_SHARE_READ, 
			NULL, 
			OPEN_EXISTING|OPEN_ALWAYS, 
			FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN, 
			NULL);

		const string source = build_value(iconSource, index);
		ret = write_ini(iniFile, SECTION_NAME, "IconResource", source);
		ERROR_CHECK(ret,TRUE);

		ret = do_change(path, iniFile);
		ERROR_CHECK(ret, TRUE);
		notify_changed(path.c_str());
		return ret;
	}

	XFILE_API BOOL set_folder_show_name( const string& path, const string& name )
	{
		BOOL ret = TRUE;
		const string iniFile = path + "/" + CONFIT_FILE;

		ret = write_ini(iniFile, SECTION_NAME, "LocalizedResourceName", name);
		ERROR_CHECK(ret, TRUE);

		ret = PathMakeSystemFolderA(path.c_str());
		ERROR_CHECK(ret, TRUE);

		ret = do_change(path, iniFile);
		ERROR_CHECK(ret, TRUE);

		notify_changed(path);
		return ret;
	}

	XFILE_API BOOL set_folder_show_name( const string& path, const string& resource, int index )
	{
		BOOL ret = TRUE;
		const string iniFile = path + "/" + CONFIT_FILE;

		const string val = build_value(resource, index);
		ret = write_ini(iniFile, SECTION_NAME, "LocalizedResourceName", val);
		ERROR_CHECK(ret,TRUE);

		ret = do_change(path, iniFile);
		ERROR_CHECK(ret, TRUE);
		notify_changed(path.c_str());
		return ret;
	}

	XFILE_API BOOL set_folder_infotip( const string& path, const string infotip )
	{
		BOOL ret = TRUE;
		const string iniFile = path + "/" + CONFIT_FILE;
		ret = write_ini(iniFile, SECTION_NAME, "InfoTip", infotip);
		ERROR_CHECK(ret, TRUE);

		ret = do_change(path, iniFile);
		ERROR_CHECK(ret, TRUE);

		notify_changed(path);
		return ret;
	}

	XFILE_API BOOL set_folder_infotip( const string& path, const string& resource, int index )
	{
		BOOL ret = TRUE;
		const string iniFile = path + "/" + CONFIT_FILE;

		string val = build_value(resource, index);
		ret = write_ini(iniFile, SECTION_NAME, "InfoTip", val);
		ERROR_CHECK(ret, TRUE);

		ret = do_change(path, iniFile);
		ERROR_CHECK(ret, TRUE);

		notify_changed(path);
		return ret;
	}

	void notify_changed(const string& path)
	{
		if(::PathIsDirectoryA(path.c_str()) == TRUE )
		{
			::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATH, path.c_str(), NULL);
		}else
		{
			::SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH, path.c_str(), NULL);
		}
	}
}