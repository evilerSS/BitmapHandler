#pragma once
#include "CMyBitmap.h"

class CBitmapHandle
{
public:
	CBitmapHandle();
	//���ļ�ͼƬfileName�е���Ϣת�洢��myBitmap�в�����CMyBitmap����ָ��
	CMyBitmap*  ReadBitmapFile(char *fileName);
	//������ԭɫ�����ͼƬ
	bool SeparateRGB(CMyBitmap* OriginaLBmp, char* outputFileNameR, char* outputFileNameG, char* outputFileNameB);
	//λͼ�ҶȻ�(8λ)�����
	bool GrayscaleBitmap(CMyBitmap* OriginaLBmp, char* outputFileName);
	//(8λ)�Ҷ�ͼ��ɫ�����
	bool GrayscaleAntiColor(CMyBitmap* OriginaLBmp, char* outputFileName);
};

