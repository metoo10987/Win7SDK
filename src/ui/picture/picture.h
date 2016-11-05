#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*	该类基于MFC， 可以将一个图片绘制到一个静态控件*/
/*	step_1: 通过这个类定义一个静态控件变量，与静态控件绑定*/
/* 	step_2: 调用 load_picture接口加载图片*/

#include <string>
using namespace std;

class picture : public CStatic
{
	DECLARE_DYNAMIC(picture)

public:
	picture();
	virtual ~picture();
	void load_picture(string picture_path);		//加载图片，picture_path图片文件的路径
	void load_picture(void* p_data,int len);		//加载图片，p_data指向图片的内存，len是p_data所指向的数据的长度
private:
	void do_load(string picture_path);
	void do_load(void* p_data,int len);
	string load_file(const string& pathname);
	void show_picture();
	void init_img_param();
	void init_client_param();
private:
	IPicture *m_p_picture;
	IStream *m_p_stream;
	bool m_b_load;
	HGLOBAL m_image_memory;
	OLE_XSIZE_HIMETRIC m_hm_img_width;
	OLE_YSIZE_HIMETRIC m_hm_img_height;
	long m_begin_xpos;		//在客户端绘图是 x起点
	long m_begin_ypos;		//在客户端绘图是 y起点
	long m_width;				//客户端绘制的宽度
	long m_height;				//客户端绘制的高度
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};