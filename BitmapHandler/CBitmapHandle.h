#pragma once
#include "CMyBitmap.h"

class CBitmapHandle
{
	// Cumulative Distribution Function: �ۼƷֲ�����
	// ���룺�Ҷȼ����ֵĸ����Լ��ۼӸ���
	// ����������ĻҶ�ֵ
	static int CDF(int n, float k); 
public:
	CBitmapHandle();
	//���ļ�ͼƬfileName�е���Ϣת�洢��myBitmap�в�����CMyBitmap����ָ��
	CMyBitmap*  ReadBitmapFile(char *fileName);
	//������ԭɫ�����ͼƬ
	bool SeparateRGB(CMyBitmap* originaLBmp, char* outputFileNameR, char* outputFileNameG, char* outputFileNameB);
	//λͼ�ҶȻ�(8λ)�����
	bool GrayscaleBitmap(CMyBitmap* originaLBmp, char* outputFileName);
	//(8λ)�Ҷ�ͼ��ɫ�����
	bool GrayscaleAntiColor(CMyBitmap* originaLBmp, char* outputFileName);
	//8λ�Ҷ�λͼ��ֱ��ͼ
	bool Histogram(CMyBitmap* originaLBmp, char* outputFileName);
	//ֱ��ͼ���⻯
	bool HistogramEqualization(CMyBitmap* originaLBmp, char* outputFileName);
	// ƽ����������ģ�������㣩������߽�㣩
	bool AverageHandleWithBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// ƽ����������ģ�������㣩��������߽�㣩
	bool AverageHandleWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// ��ֵ�˲�������߽�㣩
	bool MedianFilterWithBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// ��ֵ�˲���������߽�㣩
	bool MedianFilterWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName);
	// ͼ������
	bool BitmapScale(CMyBitmap* originaLBmp, char* outputFileName, float x, float y);
	// ͼ��ƽ��
	bool BitmapTranslation(CMyBitmap* originaLBmp, char* outputFileName, int x, int y);
	// ͼ����
	bool BitmapMirror(CMyBitmap* originaLBmp, char* outputFileName);
	// ͼ����ת
	bool BitmapRotate(CMyBitmap* originaLBmp, char* outputFileName, int angle);
	// ��ֵ�ָ����T
	bool ThresholdDivisionT(CMyBitmap* originaLBmp, char* outputFileName, int t);
	// ��ֵ�ָ������ֵ��
	bool ThresholdDivisionIteration(CMyBitmap* originaLBmp, char* outputFileName);
	// ��ֵ�ָOtsu
	bool ThresholdDivisionOtsu(CMyBitmap* originaLBmp, char* outputFileName);
	// ��ֵ�����������
	int IterationT(int *grayNum, int oldT);
	// �������ӵ������������
	bool RegionGrowingWithSeed(CMyBitmap* originaLBmp, char* outputFileName, POINT seedPoint, int t);
	// �����ӵ����������
	bool RegionGrowingWithoutSeed(CMyBitmap* originaLBmp, char* outputFileName);
	// �ж��Ƿ�����һ����ν��
	//bool isSatisfyConsistency(BYTE* data, int width, int height);
};

