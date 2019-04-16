#include <Iostream>
#include <stdio.h>
#include "CMyBitmap.h"
#include "CBitmapHandle.h"
using namespace std;

int main() {
	char fileNameOriginal[] = "./bitmaps/test.bmp";	//Ҫ�����ԭͼ��
	char fileNameR[] = "./bitmaps/bitmap_r.bmp";		//����rgb�е�R�γɵ�ͼ��
	char fileNameG[] = "./bitmaps/bitmap_g.bmp";		//����rgb�е�G�γɵ�ͼ��
	char fileNameB[] = "./bitmaps/bitmap_b.bmp";		//����rgb�е�b�γɵ�ͼ��
	char fileNameGray[] = "./bitmaps/bitmap_gray.bmp";	//8λ�Ҷ�ͼ��
	char fileNameAntiColor[] = "./bitmaps/bitmap_gray_anti_color.bmp";//8λ�Ҷȷ�ɫͼ��
	char fileNameHistogram[] = "./bitmaps/bitmap_histogram.bmp";//8λ�Ҷ�ͼ��ֱ��ͼ
	char fileNameHistogram2[] = "./bitmaps/bitmap_histogram2.bmp";//8λ�Ҷ�ͼ�ľ��⻯���ֱ��ͼ
	char fileNameEqualization[] = "./bitmaps/bitmap_equalization.bmp";//8λ�Ҷ�ͼ�ľ��⻯
	CMyBitmap		*myBitmap, *myGrayBitmap,*myGrayBitmap2;		//Ҫ�����ͼ�����
	CBitmapHandle	myBitmapHandler;					//ͼ�����߶���

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
	system("pause");
	return 0;
}

