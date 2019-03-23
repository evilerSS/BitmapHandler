#include "CBitmapHandle.h"
#include <Iostream>
#include <fstream>
using namespace std;

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
		return false;
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
	cout << "λͼ�����ɹ���" << endl;
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
	cout << "λͼ��ԭɫ����ɹ���" << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_r);
	fclose(pf_g);
	fclose(pf_b);
	return true;
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
	cout << "�Ҷ�λͼ��ɫ�ɹ���" << endl;
	//�رմ򿪵�ͼ���ļ�
	fclose(pf_gray);
	return true;
}