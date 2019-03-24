#include <Iostream>
#include <stdio.h>
#include "CMyBitmap.h"
#include "CBitmapHandle.h"
using namespace std;

int main() {
	char fileNameOriginal[] = "./bitmaps/hashiqi.bmp";	//Ҫ�����ԭͼ��
	char fileNameR[] = "./bitmaps/bitmap_r.bmp";		//����rgb�е�R�γɵ�ͼ��
	char fileNameG[] = "./bitmaps/bitmap_g.bmp";		//����rgb�е�G�γɵ�ͼ��
	char fileNameB[] = "./bitmaps/bitmap_b.bmp";		//����rgb�е�b�γɵ�ͼ��
	char fileNameGray[] = "./bitmaps/bitmap_gray.bmp";	//8λ�Ҷ�ͼ��
	char fileNameAntiColor[] = "./bitmaps/bitmap_gray_anti_color.bmp";//8λ�Ҷȷ�ɫͼ��
	char fileNameHistogram[] = "./bitmaps/bitmap_histogram.bmp";//8λ�Ҷ�ͼ��ֱ��ͼ
	CMyBitmap		*myBitmap, *myGrayBitmap;			//Ҫ�����ͼ�����
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
	}
	system("pause");
	return 0;
}

