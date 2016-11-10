// picture.cpp : ʵ���ļ�
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

// picture ��Ϣ�������


void picture::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
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
	GetDC()->HIMETRICtoDP( &sz ); // ת�� MM_HIMETRIC ģʽ��λΪ MM_TEXT ���ص�λ 

	// �Զ���Ӧ���� 
	double fRatePic, fRateWnd; 
	fRatePic = (double)sz.cx / (double)sz.cy; //����ͼƬ�Ŀ�߱�
	fRateWnd = (double)rect.Width() / (double)rect.Height(); //����client rect�Ŀ�߱�

	//������ͼ�ǣ���client renct��ռ�õĿ��m_width�͸߶� m_height
	if (fRatePic > fRateWnd) 
	{	//��Ƭ�Ŀ�߱ȱ� client rect �Ĵ���ôʹ�� client rect�Ŀ�ȴ�����Ƭ�Ŀ�߱ȣ��õ��ĸ߶�ҪС��
		//client rec�ĸ߶ȣ������Ƭ�� client rect��
		m_width = rect.Width(); 
		m_height = (long)(rect.Width() / fRatePic); 
	} 
	else 
	{ 
		m_width = (long)(rect.Height() * fRatePic); 
		m_height = rect.Height(); 
	} 

	//�������ڻ�ͼ�� client rect ��ʼλ��
	if (m_width == rect.Width()) 
	{	//�����ȣ���ʼ���� client rect��0��,�߶ȵ���ʼ��λ�ڸ߶ȵ��յ����϶���֮һ��Ƭ��
		m_begin_xpos = 0; 
		m_begin_ypos = rect.Height() / 2 - m_height / 2; 
	} 
	if (m_height == rect.Height()) 
	{	//�߶���ȣ�����x���������ƶ� 1/2client���-1/2��ͼ��ȣ�ʹ��ͼƬ������
		//client ������
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
	m_image_memory=GlobalAlloc(GMEM_MOVEABLE,len);   //��ͼƬ����ȫ���ڴ�
	void *pImageMemory=GlobalLock(m_image_memory);  //�����ڴ�
	memcpy(pImageMemory,p_data,len);
	GlobalUnlock(m_image_memory);
	CreateStreamOnHGlobal(m_image_memory,false,&m_p_stream);
	OleLoadPicture(m_p_stream,0,TRUE,IID_IPicture,(LPVOID*)&m_p_picture);
	init_img_param();
	init_client_param();
}