#pragma once
#include <afxwin.h>
//���ļ�ֻΪ����鿴λͼ�Ľṹ��Ϣ
//WORDΪ�޷���16λ������������DWORDΪ�޷���32λ����������


//λͼ�ļ�ͷ(����Ϊ14���ֽ�)
typedef struct tagBITMAPFILEHEADER {
	WORD   	bfType;			//�ļ����ͣ������ǡ�BM��
	DWORD	bfSize;			//�ļ���С�������ļ�ͷ�Ĵ�С
	WORD	bfReserved1;	//������
	WORD	bfReserved2;	//������
	DWORD	bfOffBits;		//���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
} BITMAPFILEHEADER;

//λͼ��Ϣͷ
typedef struct tagBITMAPINFOHEADER {
	DWORD   biSize;			//�ýṹ�ĳ��ȣ�Ϊ40
	LONG	biWidth;		//ͼ����
	LONG	biHeight;		//ͼ��߶�
	WORD	biPlanes;		//λƽ����������Ϊ1
	WORD	biBitCount;		//��ɫλ��
	DWORD   biCompression;	//�Ƿ�ѹ��
	DWORD   biSizeImage;	//ʵ��λͼ����ռ�õ��ֽ���
	LONG	biXPelsPerMeter;//Ŀ���豸ˮƽ�ֱ���
	LONG	biYPelsPerMeter;//Ŀ���豸��ֱ�ֱ���
	DWORD   biClrUsed;		//ʵ��ʹ�õ���ɫ��
	DWORD   biClrImportant;	//ͼ������Ҫ����ɫ��
} BITMAPINFOHEADER;

//��ɫ�壨Palette��
typedef struct tagRGBQUAD {
	BYTE  rgbBlue;   		//����ɫ����ɫ����
	BYTE  rgbGreen;			//����ɫ����ɫ����
	BYTE  rgbRed;			//����ɫ�ĺ�ɫ����
	BYTE  rgbReserved;		//����ֵ
} RGBQUAD;