#ifndef _XFILE_API_MACRO_H
#define _XFILE_API_MACRO_H

#ifndef XFILE_API

#if defined(XFILEHELPER_EXPORTS)
#define XFILE_API     __declspec(dllexport)
#else
#define XFILE_API     __declspec(dllimport)
#endif	//XFILE_EXPORTS

#endif //XFILE_API

#endif //_XFILE_API_MACRO_H