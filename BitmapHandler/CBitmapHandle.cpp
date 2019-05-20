#include "CBitmapHandle.h"
#include <Iostream>
#include <fstream>
#include <math.h>

using namespace std;

int CBitmapHandle::CDF(int n, float k)
{ 
	return 255 * k;
}

CBitmapHandle::CBitmapHandle()
{
}

CMyBitmap* CBitmapHandle::ReadBitmapFile(char *fileName)
{
	FILE*		pf_rgb;							//�ļ�ָ��
	CMyBitmap*	returnBitmap = new CMyBitmap;	//Ҫ���ص�ͼ�����
	pf_rgb = fopen(fileName, "rb");				//�����ƶ���ʽ��ָ����ͼ���ļ�
	if (pf_rgb == NULL) {
		cout << "�ļ�\"" << fileName << "\"��ʧ�ܣ������ļ����Ƿ���ȷ�����ļ��Ƿ����!" << endl;
		return NULL;
	}
	cout << "λͼ�����У����Ժ�......" << endl;
	//��ȡ�ļ�ͷ��Ϣ
	fread(&returnBitmap->m_file_head, sizeof(BITMAPFILEHEADER),1, pf_rgb);
	//��ȡ��Ϣͷ��Ϣ
	fread(&returnBitmap->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_rgb);
	//λͼλ���
	int biBitCount = returnBitmap->m_info_head.biBitCount;
	//��ȡʵ��ʹ�õ���ɫ�����Ϊ0����ʾͼ��Ϊ���ɫͼ��
	int f_biClrUsed = returnBitmap->m_info_head.biClrUsed;
	if (f_biClrUsed != 0) {
		//��ȡ��ɫ����Ϣ
		returnBitmap->m_rgbquad = new RGBQUAD[f_biClrUsed];
		for (int i = 0; i < f_biClrUsed; i++)
		{
			fread(&returnBitmap->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_rgb);
		}
	}
	//����λͼʵ�ʴ洢ʵ���������õ��ֽ���
	returnBitmap->m_info_head.biSizeImage = (returnBitmap->m_info_head.biWidth * (biBitCount / 8)+3)/4*4 * returnBitmap->m_info_head.biHeight;
	//��ȡʵ������
	returnBitmap->m_factdata = new BYTE[returnBitmap->m_info_head.biWidth * (biBitCount/8) * returnBitmap->m_info_head.biHeight];
	//��λ�ֽ���
	int platoon_bit = returnBitmap->m_info_head.biSizeImage / returnBitmap->m_info_head.biHeight 
						- returnBitmap->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < returnBitmap->m_info_head.biHeight; row++)
	{
		for (int column = 0; column < returnBitmap->m_info_head.biWidth * (biBitCount / 8); column++)
		{
			fread(&returnBitmap->m_factdata[returnBitmap->m_info_head.biWidth * (biBitCount / 8) * row + column], sizeof(BYTE), 1, pf_rgb);
		}
		//������λ��Ϣ
		fseek(pf_rgb, sizeof(BYTE)*platoon_bit, SEEK_CUR);
	}
	cout << "λͼ�����ɹ���" << endl << endl;
	return returnBitmap;
}
bool CBitmapHandle::SeparateRGB(CMyBitmap* originaLBmp, char* outputFileNameR, char* outputFileNameG, char* outputFileNameB) {

	cout << "λͼ��ԭɫ�����У����Ժ�......" << endl;
	FILE *pf_r, *pf_g, *pf_b;
	pf_r = fopen(outputFileNameR, "wb");		//�����ƶ���ʽ����ֻ��R��ͼ���ļ�
	pf_g = fopen(outputFileNameG, "wb");		//�����ƶ���ʽ����ֻ��G��ͼ���ļ�
	pf_b = fopen(outputFileNameB, "wb");		//�����ƶ���ʽ����ֻ��B��ͼ���ļ�
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_r);
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_g);
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_b);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_r);
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_g);
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_b);

	//д��ʵ�ʵ�ͼ������
	//λ���
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	BYTE zero = 0;
	for (int row = 0; row < originaLBmp->m_info_head.biHeight; row++) {
		for (int column = 0; column < originaLBmp->m_info_head.biWidth * 3; column += 3) {
			//дBɫͼ��
			fwrite(&originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + column], sizeof(BYTE), 1, pf_b);
			fwrite(&zero, sizeof(BYTE), 1, pf_b);
			fwrite(&zero, sizeof(BYTE), 1, pf_b);
			//дGɫͼ��
			fwrite(&zero, sizeof(BYTE), 1, pf_g);
			fwrite(&originaLBmp->m_factdata[row*originaLBmp->m_info_head.biWidth * 3 + column + 1], sizeof(BYTE), 1, pf_g);
			fwrite(&zero, sizeof(BYTE), 1, pf_g);
			//дRɫͼ��
			fwrite(&zero, sizeof(BYTE), 1, pf_r);
			fwrite(&zero, sizeof(BYTE), 1, pf_r);
			fwrite(&originaLBmp->m_factdata[row*originaLBmp->m_info_head.biWidth * 3 + column + 2], sizeof(BYTE), 1, pf_r);
		}
		//����
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_b);
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_g);
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_r);
	}
	cout << "λͼ��ԭɫ����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_r);
	fclose(pf_g);
	fclose(pf_b);
	return true;
}
bool CBitmapHandle::GrayscaleBitmap(CMyBitmap* originaLBmp, char* outputFileName) {
	cout << "λͼ�ҶȻ��У����Ժ�......" << endl;
	FILE *pf_gray;
	pf_gray = fopen(outputFileName, "wb");	//������д��ʽ����8λ�Ҷ�ͼ���ļ�
	CMyBitmap *targetBitmap = new CMyBitmap();
	//�����ļ�ͷ
	targetBitmap->m_file_head = originaLBmp->m_file_head;
	//��������ô��ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
	DWORD targetBfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	targetBitmap->m_file_head.bfOffBits = targetBfOffBits;
	//д���ļ�ͷ
	fwrite(&targetBitmap->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_gray);

	//������Ϣͷ
	targetBitmap->m_info_head = originaLBmp->m_info_head;
	targetBitmap->m_info_head.biBitCount = 8;
	targetBitmap->m_info_head.biSizeImage = (originaLBmp->m_info_head.biWidth * (targetBitmap->m_info_head.biBitCount / 8) + 3) / 4
											* 4 * originaLBmp->m_info_head.biHeight; 
	targetBitmap->m_info_head.biClrUsed = 256;
	targetBitmap->m_info_head.biClrImportant = 256;
	//д����Ϣͷ
	fwrite(&targetBitmap->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_gray);

	//���ú�д���ɫ��
	targetBitmap->m_rgbquad = new RGBQUAD[256];
	for (int i = 0; i < 256; i++)
	{
		RGBQUAD newRgbQ;
		newRgbQ.rgbBlue = i;
		newRgbQ.rgbGreen = i;
		newRgbQ.rgbRed = i;
		newRgbQ.rgbReserved = 0;
		targetBitmap->m_rgbquad[i] = newRgbQ;
		fwrite(&targetBitmap->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_gray);
	}
	//λ���
	int biBitCount = targetBitmap->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = targetBitmap->m_info_head.biSizeImage / targetBitmap->m_info_head.biHeight
		- targetBitmap->m_info_head.biWidth * (biBitCount / 8);
	//���ú�д��λͼʵ������
	targetBitmap->m_factdata = new BYTE[targetBitmap->m_info_head.biWidth * targetBitmap->m_info_head.biHeight];
	for (int row = 0; row < targetBitmap->m_info_head.biHeight; row++) {
		for (int column = 0; column < targetBitmap->m_info_head.biWidth; column++) {
			//����Ҷ�ֵ
			BYTE gray_value =
					 (int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + 3 * column]) * 11
					+ int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + 3 * column + 1]) * 59
					+ int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + 3 * column + 2]) * 30) / 100;
			//ʵ��ֵ�洢
			targetBitmap->m_factdata[row * targetBitmap->m_info_head.biWidth + column] = gray_value;
			//д���ɫ
			fwrite(&gray_value, sizeof(BYTE), 1, pf_gray);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)* platoon_bit, 1, pf_gray);
	}
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_gray);

	return true;
}
bool CBitmapHandle::GrayscaleAntiColor(CMyBitmap* originaLBmp, char* outputFileName) {
	FILE *pf_gray;
	cout << "�Ҷ�λͼ��ɫ�У����Ժ�......" << endl;
	pf_gray = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_gray);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_gray);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_gray);
	}
	//λ���
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	for (int row = 0; row < originaLBmp->m_info_head.biHeight; row++) {
		for (int column = 0; column < originaLBmp->m_info_head.biWidth; column++) {
			//д��Ҷ�ͼ��
			BYTE grayValue = 255 - int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth + column]);
			fwrite(&grayValue, sizeof(BYTE), 1, pf_gray);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_gray);
	}
	cout << "�Ҷ�λͼ��ɫ�ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_gray);
	return true;
}
bool CBitmapHandle::Histogram(CMyBitmap* originaLBmp, char* outputFileName) {
	FILE *pf_bmp;
	int pBuffer[256] = {0};			//���Ҷȼ�����������
	int maxGrayValue;			//ֱ��ͼ�Ҷ����������
	float heightCompress = 1.0f;//�߶�ѹ������
	int bitHeight = 250;		//ֱ��ͼ�߶�
	int bitWidth = 256;			//ֱ��ͼ���
	cout << "ͼ��ֱ��ͼ�����У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�
	//���ص�������
	int pixelSum = int(originaLBmp->m_info_head.biWidth * originaLBmp->m_info_head.biHeight);
	//�����������ص�
	for (int i = 0; i< pixelSum; i++) {
		int grayValue = int(originaLBmp->m_factdata[i]);//��ǰ���ص�ĻҶ�ֵ
		pBuffer[grayValue]++;							//grayValue�Ҷȵ���������+1
	}
	//Ѱ�����߶�
	maxGrayValue = 0;
	for (int i = 0; i < 256; i++) {
		maxGrayValue = pBuffer[i] > maxGrayValue ? pBuffer[i] : maxGrayValue;
	}
	//����߶�ѹ������
	heightCompress = float(maxGrayValue) / float(bitHeight);
	//���¼�����Ҷ�����������
	for (int i = 0; i < 256; i++) {
		pBuffer[i] = float(pBuffer[i]) / heightCompress;
	}
	//д���ļ�ͷ��Ϣ
	BITMAPFILEHEADER bitmap_file_head;
	bitmap_file_head.bfType = 'MB';
	bitmap_file_head.bfReserved1 = 0;
	bitmap_file_head.bfReserved2 = 0;
	bitmap_file_head.bfSize = sizeof(BITMAPFILEHEADER)
									+ sizeof(BITMAPINFOHEADER) 
									+ sizeof(RGBQUAD) * 2 
									+ sizeof(BYTE) * 256 * bitHeight;
	bitmap_file_head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;
	fwrite(&bitmap_file_head,sizeof(BITMAPFILEHEADER),1, pf_bmp);
	//д����Ϣͷ��Ϣ
	BITMAPINFOHEADER bitmap_info_head;
	bitmap_info_head.biSize = 40;
	bitmap_info_head.biWidth = bitWidth;
	bitmap_info_head.biHeight = bitHeight;
	bitmap_info_head.biPlanes = 1;
	bitmap_info_head.biBitCount = 8;
	bitmap_info_head.biCompression = 0;
	bitmap_info_head.biSizeImage = bitWidth * bitHeight;
	bitmap_info_head.biClrUsed = 2;
	bitmap_info_head.biClrImportant = 2;
	fwrite(&bitmap_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	RGBQUAD gbquad[2];
	//gbquad[0]Ϊ����ɫ��gbquad[1]Ϊ����ɫ
	for (int i = 0; i < 2; i++) {
		gbquad[i].rgbBlue = i * 255;
		gbquad[i].rgbGreen = i * 255;
		gbquad[i].rgbRed = i * 255;
		gbquad[i].rgbReserved = i * 255;
		fwrite(&gbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (pBuffer[column] > 0) {
				gbquadIndex = 0;
				pBuffer[column]--; 
			}
			else {
				gbquadIndex = 1;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
	}
	cout << "ͼ��ֱ��ͼ�����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);
	return true;
}
bool CBitmapHandle::HistogramEqualization(CMyBitmap* originaLBmp, char* outputFileName) {
	FILE *pf_bmp;
	int pxielNum[256];
	double pRobabilityFunc[256];//�ۼƸ��ʷֲ�����
	for (int i = 0; i < 256; i++) {
		pxielNum[i] = 0;
		pRobabilityFunc[i] = 0.0f;
	}

	cout << "�Ҷ�ͼ���⻯�У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�
	
	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//����Ҷ����ص�
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			int index = originaLBmp->m_factdata[row * bitWidth + column];
			pxielNum[index]++;
		}
	}
	//�����ۼƷֲ�����
	pRobabilityFunc[0] = double(pxielNum[0]) / (bitHeight* bitWidth);
	for (int i = 1; i < 256; i++) {
		pRobabilityFunc[i] = pRobabilityFunc[i-1]+ double(pxielNum[i]) / (bitHeight* bitWidth);
	}
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//д��Ҷ�ͼ��
			BYTE gbquadIndex = 255 * pRobabilityFunc[int(originaLBmp->m_factdata[row * bitWidth + column])];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "�Ҷ�ͼ���⻯�ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);
	return true;
}


// ƽ����������ģ�������㣩������߽�㣩
bool CBitmapHandle::AverageHandleWithBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "�Ҷ�ͼ������(ƽ�����������߽�)�����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);

	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//д��Ҷ�ͼ��
			BYTE gbquadIndex;
			int filter[9];
			//����ƽ��ֵ
			filter[0] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth? row:(row + 1)) * bitWidth + ((column - 1) < 0 ? column: (column - 1))]);
			filter[1] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + column]);
			filter[2] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			filter[3] = int(originaLBmp->m_factdata[row * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[4] = int(originaLBmp->m_factdata[row * bitWidth + column]);
			filter[5] = int(originaLBmp->m_factdata[row * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			filter[6] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[7] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + column]);
			filter[8] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			int sum = 0;
			for (int i = 0; i < 9; i++)
			{
				sum += filter[i];
			}
			int result = sum / 9;
			if (result - 255 > 0) {
				result = 255;
			}
			if (result < 0) {
				result = 0;
			}
			gbquadIndex = result;

			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "�Ҷ�ͼ����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}

// ƽ����������ģ�������㣩��������߽�㣩
bool CBitmapHandle::AverageHandleWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "�Ҷ�ͼ������(ƽ�������������߽�)�����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);

	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//д��Ҷ�ͼ��
			BYTE gbquadIndex;
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {//�߽磺������
				gbquadIndex = int(originaLBmp->m_factdata[row * bitWidth + column]);
			}
			else {//�Ǳ߽磺ƽ������
				// ����ƽ��ֵ
				int filter[9];
				filter[0] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + (column - 1)]);
				filter[1] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + column]);
				filter[2] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + (column + 1)]);
				filter[3] = int(originaLBmp->m_factdata[row * bitWidth + (column - 1)]);
				filter[4] = int(originaLBmp->m_factdata[row * bitWidth + column]);
				filter[5] = int(originaLBmp->m_factdata[row * bitWidth + (column + 1)]);
				filter[6] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + (column - 1)]);
				filter[7] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + column]);
				filter[8] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + (column + 1)]);
				int sum = 0;
				for (int i = 0; i < 9; i++)
				{
					sum += filter[i];
				}
				int result = sum / 9;
				if (result - 255 > 0) {
					result = 255;
				}
				if (result < 0) {
					result = 0;
				}
				gbquadIndex = result;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "�Ҷ�ͼ����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}

// ��ֵ�˲�������߽�㣩
bool CBitmapHandle::MedianFilterWithBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "�Ҷ�ͼ������(��ֵ�˲��������߽�)�����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);

	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//д��Ҷ�ͼ��
			BYTE gbquadIndex;
			int filter[9];
			//����ƽ��ֵ
			filter[0] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[1] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + column]);
			filter[2] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			filter[3] = int(originaLBmp->m_factdata[row * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[4] = int(originaLBmp->m_factdata[row * bitWidth + column]);
			filter[5] = int(originaLBmp->m_factdata[row * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			filter[6] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[7] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + column]);
			filter[8] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			//����
			for (int i = 0; i < 8; i++) {
				for (int j = i + 1; j < 9; j++) {
					if (filter[i] > filter[j]) {
						int temp = filter[i];
						filter[i] = filter[j];
						filter[j] = temp;
					}
				}
			}
			gbquadIndex = filter[4];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "�Ҷ�ͼ����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}

// ��ֵ�˲���������߽�㣩
bool CBitmapHandle::MedianFilterWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "�Ҷ�ͼ������(��ֵ�˲����������߽�)�����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);

	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//д��Ҷ�ͼ��
			BYTE gbquadIndex;
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {//�߽磺������
				gbquadIndex = int(originaLBmp->m_factdata[row * bitWidth + column]);
			}
			else {//�Ǳ߽磺ƽ������
				// ����ƽ��ֵ
				int filter[9];
				filter[0] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + (column - 1)]);
				filter[1] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + column]);
				filter[2] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + (column + 1)]);
				filter[3] = int(originaLBmp->m_factdata[row * bitWidth + (column - 1)]);
				filter[4] = int(originaLBmp->m_factdata[row * bitWidth + column]);
				filter[5] = int(originaLBmp->m_factdata[row * bitWidth + (column + 1)]);
				filter[6] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + (column - 1)]);
				filter[7] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + column]);
				filter[8] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + (column + 1)]);
				//����
				for (int i = 0; i < 8; i++) {
					for (int j = i + 1; j < 9; j++) {
						if (filter[i] > filter[j]) {
							int temp = filter[i];
							filter[i] = filter[j];
							filter[j] = temp;
						}
					}
				}
				gbquadIndex = filter[4];
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "�Ҷ�ͼ����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ͼ������
bool CBitmapHandle::BitmapScale(CMyBitmap* originaLBmp, char* outputFileName, float x, float y)
{
	FILE *pf_bmp;
	cout << "ͼ�������У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	BITMAPINFOHEADER new_info_head = originaLBmp->m_info_head;
	new_info_head.biWidth = LONG(new_info_head.biWidth * x);
	new_info_head.biHeight = LONG(new_info_head.biHeight * y);
	new_info_head.biSizeImage = (new_info_head.biWidth * (new_info_head.biBitCount / 8) + 3) / 4 * 4 * new_info_head.biHeight;
	fwrite(&new_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = new_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = new_info_head.biSizeImage / new_info_head.biHeight
		- new_info_head.biWidth * (biBitCount / 8);
	BYTE zero = 0;
	int bitHeight = new_info_head.biHeight;		//��ͼ�߶�
	int bitWidth = new_info_head.biWidth;		//��ͼ���
	//��С
	float cX = 1 / x;
	float cY = 1 / y;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			BYTE gbquadIndex = originaLBmp->m_factdata[int(cY * row) * originaLBmp->m_info_head.biWidth + int(cX * column)];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}

	cout << "ͼ�����ųɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ͼ��ƽ��
bool CBitmapHandle::BitmapTranslation(CMyBitmap* originaLBmp, char* outputFileName, int x, int y)
{
	FILE *pf_bmp;
	cout << "ͼ��ƽ���У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			int rowIndex = row - y;
			int columnIndex = column - x;
			BYTE gbquadIndex = 0;
			if (rowIndex < 0 || rowIndex >= bitHeight || columnIndex < 0 || columnIndex >= bitWidth) {
				gbquadIndex = 0;
			}
			else {
				gbquadIndex = originaLBmp->m_factdata[rowIndex * bitWidth + columnIndex];
			}			
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "ͼ��ƽ�Ƴɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ͼ����
bool CBitmapHandle::BitmapMirror(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "ͼ����ͼ�����У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			BYTE gbquadIndex = originaLBmp->m_factdata[row * bitWidth + (bitWidth - column)];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "ͼ����ͼ���ɳɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ͼ����ת
bool CBitmapHandle::BitmapRotate(CMyBitmap* originaLBmp, char* outputFileName, int angle)
{
	FILE *pf_bmp;
	cout << "ͼ����ת�У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	POINT center;
	center.x = LONG(originaLBmp->m_info_head.biWidth / 2);
	center.y = LONG(originaLBmp->m_info_head.biHeight / 2);
	float byta = angle;
	BYTE gbquadIndex;

	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			int xOriginal = column * cos(byta * 0.01745) - row * sin(byta * 0.01745);
			int yOriginal = column * sin(byta * 0.01745) + row * cos(byta * 0.01745);

			if (xOriginal < 0 || xOriginal >= bitWidth || yOriginal < 0 || yOriginal >= bitHeight) {
				gbquadIndex = 0;
			}
			else {
				gbquadIndex = originaLBmp->m_factdata[yOriginal * bitWidth + xOriginal];
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "ͼ����ת�ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ��ֵ�ָ����T
bool CBitmapHandle::ThresholdDivisionT(CMyBitmap* originaLBmp, char* outputFileName, int t)
{
	FILE *pf_bmp;
	cout << "ͼ����ֵ�ָ������TΪ "<< t <<" ���У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	BYTE gbquadIndex;

	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {	
			if (originaLBmp->m_factdata[row * bitWidth + column] > t) {
				gbquadIndex = 0;
			}
			else {
				gbquadIndex = 255;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "ͼ����ֵ�ָ����T���ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ��ֵ�ָ������ֵ��
bool CBitmapHandle::ThresholdDivisionIteration(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "ͼ����ֵ�ָ������ֵ�����У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	int t = 128;		//��ʼ����ֵ
	int pGrayNum[256] = {0};	//���Ҷȼ�����������
	//�Ҷ��ܶȺ���ͳ��
	//���ص�������
	int pixelSum = int(originaLBmp->m_info_head.biWidth * originaLBmp->m_info_head.biHeight);
	//�����������ص�
	for (int i = 0; i < pixelSum; i++) {
		int grayValue = int(originaLBmp->m_factdata[i]);//��ǰ���ص�ĻҶ�ֵ
		pGrayNum[grayValue]++;							//grayValue�Ҷȵ���������+1
	}
	//�������������ֵ
	t = IterationT(pGrayNum, 128);
	cout << "�����ֵΪ��" << t << endl;
	BYTE gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (originaLBmp->m_factdata[row * bitWidth + column] > t) {
				gbquadIndex = 0;
			}
			else {
				gbquadIndex = 255;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "ͼ����ֵ�ָ������ֵ�����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ��ֵ�ָOtsu
bool CBitmapHandle::ThresholdDivisionOtsu(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "ͼ����ֵ�ָOtsu�����У����Ժ�......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//�����ƶ���ʽ����8λ�Ҷ�ͼ���ļ�

	int bitHeight = originaLBmp->m_info_head.biHeight;		//ԭͼ�߶�
	int bitWidth = originaLBmp->m_info_head.biWidth;		//ԭͼ���
	//���ͼƬ
	//д���ļ�ͷ
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//д����Ϣͷ
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//д���ɫ��
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//д����ʵ����
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//��λ�ֽ���
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	////�Ҷ��ܶȺ���ͳ��
	////���ص�������
	//int pixelSum = int(originaLBmp->m_info_head.biWidth * originaLBmp->m_info_head.biHeight);
	////�����������ص�
	//for (int i = 0; i < pixelSum; i++) {
	//	int grayValue = int(originaLBmp->m_factdata[i]);//��ǰ���ص�ĻҶ�ֵ
	//	pGrayNum[grayValue]++;							//grayValue�Ҷȵ���������+1
	//}
	////�������������ֵ
	//t = IterationT(pGrayNum, 128);
	int t = 0;				//�����ֵ
	float pGrayNum[256] = { 0 };	//���Ҷȼ�����������
	float wk[256] = { 0 };	//�Ҷȼ�0��k�����صĳ��ָ���
	float uk[256] = { 0 };	//ƽ���Ҷ�
	float variance = 0;		//��䷽��
	float maxVariance = variance;//�����䷽��
	//���ص�������
	int pixelSum = int(originaLBmp->m_info_head.biWidth * originaLBmp->m_info_head.biHeight);

	//�����������ص�
	for (int i = 0; i < pixelSum; i++) {
		int grayValue = int(originaLBmp->m_factdata[i]);//��ǰ���ص�ĻҶ�ֵ
		pGrayNum[grayValue]++;							//grayValue�Ҷȵ���������+1
	}

	for (int i = 0; i < 256; i++) {
		if (i == 0) {
			wk[i] = pGrayNum[i] / pixelSum;
			uk[i] = i * wk[i];
		}
		else {
			wk[i] = pGrayNum[i] / pixelSum + wk[i - 1];
			for (int j = 0; j < i+1; j++)
			{
				uk[i] += j * pGrayNum[j] / pixelSum;
			}
		}
	}
	for (int i = 1; i < 255; i++)
	{
		variance = wk[i] * (wk[255] - wk[i]) * pow( uk[i] * (uk[255] - uk[i]), 2);
		if (variance > maxVariance) {//�����ǰ���������󷽲�
			maxVariance = variance;	//������󷽲�
			t = i;					//���������ֵ
		}
	}
	//��ֵ�������
	cout << "�����ֵΪ��" << t << endl;
	BYTE gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (originaLBmp->m_factdata[row * bitWidth + column] > t) {
				gbquadIndex = 0;
			}
			else {
				gbquadIndex = 255;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//����
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "ͼ����ֵ�ָOtsu�����ɹ���" << endl << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_bmp);

	return true;
}


// ��ֵ�����������
int CBitmapHandle::IterationT(int *grayNum, int oldT)
{
	int newT = 0;
	int u1 = 0, u2 = 0;
	int numerator = 0, denominator = 0;
	for (int i = 0; i < 128; i++)
	{	
		numerator += i * grayNum[i];
		denominator += grayNum[i];
	}
	u1 = numerator / denominator;
	numerator = 0;
	denominator = 0;
	for (int i = 128; i < 255; i++)
	{
		numerator += i * grayNum[i];
		denominator += grayNum[i];
	}
	u2 = numerator / denominator;
	newT = 0.5 * (u1 + u2);

	if (newT - oldT < 2) {
		return newT;
	}
	else {
		return IterationT(grayNum, newT);
	}
}

