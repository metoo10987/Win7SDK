#pragma once
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*	�������MFC�� ���Խ�һ��ͼƬ���Ƶ�һ����̬�ؼ�*/
/*	step_1: ͨ������ඨ��һ����̬�ؼ��������뾲̬�ؼ���*/
/* 	step_2: ���� load_picture�ӿڼ���ͼƬ*/

#include <string>
using namespace std;

class picture : public CStatic
{
	DECLARE_DYNAMIC(picture)

public:
	picture();
	virtual ~picture();
	void load_picture(string picture_path);		//����ͼƬ��picture_pathͼƬ�ļ���·��
	void load_picture(void* p_data,int len);		//����ͼƬ��p_dataָ��ͼƬ���ڴ棬len��p_data��ָ������ݵĳ���
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
	long m_begin_xpos;		//�ڿͻ��˻�ͼ�� x���
	long m_begin_ypos;		//�ڿͻ��˻�ͼ�� y���
	long m_width;				//�ͻ��˻��ƵĿ��
	long m_height;				//�ͻ��˻��Ƶĸ߶�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};