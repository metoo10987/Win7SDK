// picture.cpp : 实现文件
//

#include "stdafx.h"
#include "picture.h"

#include <fstream>
using namespace std;

// picture

IMPLEMENT_DYNAMIC(picture, CStatic)

picture::picture() : m_b_load(false)
{
	::CoInitialize(NULL);
}

picture::~picture()
{
}

BEGIN_MESSAGE_MAP(picture, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// picture 消息处理程序


void picture::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	show_picture();
}

void picture::load_picture( string picture_path )
{
	do_load(picture_path);
	m_b_load = true;
	show_picture();
}

void picture::load_picture( void* p_data,int len )
{
	do_load(p_data,len);
	m_b_load = true;
	show_picture();
}

void picture::init_client_param()
{
	CRect rect;
	GetClientRect(&rect); 
	CSize sz( m_hm_img_width, m_hm_img_height); 
	GetDC()->HIMETRICtoDP( &sz ); // 转换 MM_HIMETRIC 模式单位为 MM_TEXT 像素单位 

	// 自动适应窗口 
	double fRatePic, fRateWnd; 
	fRatePic = (double)sz.cx / (double)sz.cy; //计数图片的宽高比
	fRateWnd = (double)rect.Width() / (double)rect.Height(); //计数client rect的宽高比

	//计数绘图是，在client renct中占用的宽度m_width和高度 m_height
	if (fRatePic > fRateWnd) 
	{	//照片的宽高比比 client rect 的大，那么使用 client rect的宽度处以照片的宽高比，得到的高度要小于
		//client rec的高度，因此照片在 client rect中
		m_width = rect.Width(); 
		m_height = (long)(rect.Width() / fRatePic); 
	} 
	else 
	{ 
		m_width = (long)(rect.Height() * fRatePic); 
		m_height = rect.Height(); 
	} 

	//计算用于绘图的 client rect 起始位置
	if (m_width == rect.Width()) 
	{	//宽度相等，起始点在 client rect的0点,高度的起始点位于高度的终点向上二分之一照片高
		m_begin_xpos = 0; 
		m_begin_ypos = rect.Height() / 2 - m_height / 2; 
	} 
	if (m_height == rect.Height()) 
	{	//高度相等，则在x方向向右移动 1/2client宽度-1/2绘图宽度，使得图片绘制在
		//client 的中心
		m_begin_xpos = rect.Width() / 2 - m_width / 2; 
		m_begin_ypos = 0; 
	}
}

void picture::init_img_param()
{
	m_p_picture->get_Width(&m_hm_img_width);
	m_p_picture->get_Height(&m_hm_img_height);
}

void picture::show_picture()
{
	if(!m_b_load)
		return;
	{
		HDC hdc = GetDC()->GetSafeHdc();
		m_p_picture->Render(hdc, m_begin_xpos, m_begin_ypos, m_width, m_height, 
			0, m_hm_img_height, m_hm_img_width, -m_hm_img_height, NULL); 
		DeleteDC(hdc);
	}
}

std::string picture::load_file( const string& pathname )
{
	fstream in_file;
	in_file.open(pathname.c_str(),ios::in|ios::binary);
	if(in_file.is_open())
	{
		istreambuf_iterator<char> beg(in_file),end;
		string str(beg,end);
		in_file>>str;
		in_file.close();
		return str;
	}
	return "";
}

void picture::do_load( string picture_path )
{
	string img_str = load_file(picture_path);
	load_picture((void*)img_str.c_str(),img_str.size());
}

void picture::do_load( void* p_data,int len )
{
	m_image_memory=GlobalAlloc(GMEM_MOVEABLE,len);   //给图片分配全局内存
	void *pImageMemory=GlobalLock(m_image_memory);  //锁定内存
	memcpy(pImageMemory,p_data,len);
	GlobalUnlock(m_image_memory);
	CreateStreamOnHGlobal(m_image_memory,false,&m_p_stream);
	OleLoadPicture(m_p_stream,0,TRUE,IID_IPicture,(LPVOID*)&m_p_picture);
	init_img_param();
	init_client_param();
}