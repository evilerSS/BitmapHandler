#include <Iostream>
#include <stdio.h>
#include "CMyBitmap.h"
#include "CBitmapHandle.h"
using namespace std;

int main() {
	char fileNameOriginal[] = "./bitmaps/bitmap1.bmp";	//Ҫ�����ԭͼ��1
	char fileNameOriginal2[] = "./bitmaps/bitmap2.bmp";	//Ҫ�����ԭͼ��2:��������ͼ��
	char fileNameOriginal3[] = "./bitmaps/bitmap3.bmp";	//Ҫ�����ԭͼ��3

	char fileNameR[] = "./bitmaps/1/bitmap_r.bmp";		//����rgb�е�R�γɵ�ͼ��
	char fileNameG[] = "./bitmaps/1/bitmap_g.bmp";		//����rgb�е�G�γɵ�ͼ��
	char fileNameB[] = "./bitmaps/1/bitmap_b.bmp";		//����rgb�е�b�γɵ�ͼ��
	char fileNameGray[] = "./bitmaps/1/bitmap_gray.bmp";	//8λ�Ҷ�ͼ��
	char fileNameAntiColor[] = "./bitmaps/1/bitmap_gray_anti_color.bmp";//8λ�Ҷȷ�ɫͼ��

	char fileNameHistogram[] = "./bitmaps/2/bitmap_histogram.bmp";//8λ�Ҷ�ͼ��ֱ��ͼ
	char fileNameHistogram2[] = "./bitmaps/2/bitmap_histogram2.bmp";//8λ�Ҷ�ͼ�ľ��⻯���ֱ��ͼ
	char fileNameEqualization[] = "./bitmaps/2/bitmap_equalization.bmp";//8λ�Ҷ�ͼ�ľ��⻯

	char fileNameAverageHandleWithBorder[] = "./bitmaps/3/bitmap_average_handle_with_border.bmp";//ƽ������(����߽�)���ͼ��
	char fileNameAverageHandleWithoutBorder[] = "./bitmaps/3/bitmap_average_handle_without_border.bmp";//ƽ������(������߽�)���ͼ��
	char fileNameMedianFilterWithBorder[] = "./bitmaps/3/bitmap_median_filter_with_border.bmp";//��ֵ�˲�(����߽�)���ͼ��
	char fileNameMedianFilterWithoutBorder[] = "./bitmaps/3/bitmap_median_filter_without_border.bmp";//��ֵ�˲�(������߽�)���ͼ��

	char fileNameScale[] = "./bitmaps/4/bitmap_scale.bmp";//����
	char fileNameTranslation[] = "./bitmaps/4/bitmap_translation.bmp";//ƽ��
	char fileNameMirror[] = "./bitmaps/4/bitmap_mirror.bmp";//����
	char fileNameRotate[] = "./bitmaps/4/bitmap_rotate.bmp";//����

	char fileNameThresholdDivisionT[] = "./bitmaps/5/bitmap_threshold_division_t.bmp";
	char fileNameThresholdDivisionIteration[] = "./bitmaps/5/bitmap_threshold_division_interation.bmp";
	char fileNameThresholdDivisionOtsu[] = "./bitmaps/5/bitmap_threshold_division_otsu.bmp";

	//Ҫ�����ͼ�����
	CMyBitmap		*myBitmap,		//ԭͼ1
					*myGrayBitmap,	//�Ҷ�ͼ
					*myGrayBitmap2,	//�Ҷ�ͼ2�����⻯��ģ�
					*myGrayBitmap3,	//�Ҷ�ͼ2�����⻯��ģ�
					*noiseBitmap;	//��������ͼ��

	CBitmapHandle	myBitmapHandler;//ͼ�����߶���

	//����λͼ��Ϣ�����ؽ������(CMyBitmap����ָ��)
	myBitmap = myBitmapHandler.ReadBitmapFile(fileNameOriginal);
	if (myBitmap != NULL) {
		//λͼ������ԭɫ�����
		myBitmapHandler.SeparateRGB(myBitmap, fileNameR, fileNameG, fileNameB);
		//λͼ�ҶȻ������
		myBitmapHandler.GrayscaleBitmap(myBitmap, fileNameGray);
	}
	//�����Ҷ�λͼ��Ϣ�����ؽ������
	myGrayBitmap = myBitmapHandler.ReadBitmapFile(fileNameGray);
	if (myGrayBitmap != NULL) {
		//�Ҷ�ͼ��ɫ�����
		myBitmapHandler.GrayscaleAntiColor(myGrayBitmap, fileNameAntiColor);
		//�Ҷ�ͼֱ��ͼ���
		myBitmapHandler.Histogram(myGrayBitmap, fileNameHistogram);
		//�Ҷ�ͼ���⻯
		myBitmapHandler.HistogramEqualization(myGrayBitmap, fileNameEqualization);
	}
	//�������⻯��ĻҶ�ͼ
	myGrayBitmap2 = myBitmapHandler.ReadBitmapFile(fileNameEqualization);
	if (myGrayBitmap2 != NULL) {
		//�������⻯��ĻҶ�ͼֱ��ͼ���
		myBitmapHandler.Histogram(myGrayBitmap2, fileNameHistogram2);
	}
	//�����Ҷ�ͼ
	noiseBitmap = myBitmapHandler.ReadBitmapFile(fileNameOriginal2);
	if (noiseBitmap != NULL) {
		//��������
		myBitmapHandler.AverageHandleWithBorder(noiseBitmap, fileNameAverageHandleWithBorder);
		myBitmapHandler.AverageHandleWithoutBorder(noiseBitmap, fileNameAverageHandleWithoutBorder);
		myBitmapHandler.MedianFilterWithBorder(noiseBitmap, fileNameMedianFilterWithBorder);
		myBitmapHandler.MedianFilterWithoutBorder(noiseBitmap, fileNameMedianFilterWithoutBorder);
		//�����任
		myBitmapHandler.BitmapScale(noiseBitmap, fileNameScale, 0.8, 1.2);
		myBitmapHandler.BitmapTranslation(noiseBitmap, fileNameTranslation, 100, -100); 
		myBitmapHandler.BitmapMirror(noiseBitmap, fileNameMirror);
		myBitmapHandler.BitmapRotate(noiseBitmap, fileNameRotate, -20);
	}
	//�������⻯��ĻҶ�ͼ
	myGrayBitmap3 = myBitmapHandler.ReadBitmapFile(fileNameOriginal3);
	if (myGrayBitmap3 != NULL) {
		//��ֵ�ָ�
		myBitmapHandler.ThresholdDivisionT(myGrayBitmap3, fileNameThresholdDivisionT, 128);
		myBitmapHandler.ThresholdDivisionIteration(myGrayBitmap3, fileNameThresholdDivisionIteration);
		myBitmapHandler.ThresholdDivisionOtsu(myGrayBitmap3, fileNameThresholdDivisionOtsu);
	}
	system("pause");
	return 0;
}

