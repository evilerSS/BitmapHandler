#pragma once
#include <windows.h>
class CMyBitmap
{
public:
	BITMAPFILEHEADER	m_file_head;	//�ļ�ͷ
	BITMAPINFOHEADER	m_info_head;	//��Ϣͷ
	RGBQUAD				*m_rgbquad;		//��ɫ��
	BYTE				*m_factdata;	//λͼʵ������
public:
	CMyBitmap();
	~CMyBitmap();
};

