#include "CBitmapHandle.h"
#include <Iostream>
#include <fstream>
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
	FILE*		pf_rgb;							//文件指针
	CMyBitmap*	returnBitmap = new CMyBitmap;	//要返回的图像对象
	pf_rgb = fopen(fileName, "rb");				//二进制读方式打开指定的图像文件
	if (pf_rgb == NULL) {
		cout << "文件\"" << fileName << "\"打开失败，请检查文件名是否正确或者文件是否存在!" << endl;
		return NULL;
	}
	cout << "位图解析中，请稍后......" << endl;
	//读取文件头信息
	fread(&returnBitmap->m_file_head, sizeof(BITMAPFILEHEADER),1, pf_rgb);
	//读取信息头信息
	fread(&returnBitmap->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_rgb);
	//位图位深度
	int biBitCount = returnBitmap->m_info_head.biBitCount;
	//读取实际使用的颜色（如果为0，表示图像为真彩色图像）
	int f_biClrUsed = returnBitmap->m_info_head.biClrUsed;
	if (f_biClrUsed != 0) {
		//读取调色板信息
		returnBitmap->m_rgbquad = new RGBQUAD[f_biClrUsed];
		for (int i = 0; i < f_biClrUsed; i++)
		{
			fread(&returnBitmap->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_rgb);
		}
	}
	//设置位图实际存储实际数据所用的字节数
	returnBitmap->m_info_head.biSizeImage = (returnBitmap->m_info_head.biWidth * (biBitCount / 8)+3)/4*4 * returnBitmap->m_info_head.biHeight;
	//读取实际数据
	returnBitmap->m_factdata = new BYTE[returnBitmap->m_info_head.biWidth * (biBitCount/8) * returnBitmap->m_info_head.biHeight];
	//补位字节数
	int platoon_bit = returnBitmap->m_info_head.biSizeImage / returnBitmap->m_info_head.biHeight 
						- returnBitmap->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < returnBitmap->m_info_head.biHeight; row++)
	{
		for (int column = 0; column < returnBitmap->m_info_head.biWidth * (biBitCount / 8); column++)
		{
			fread(&returnBitmap->m_factdata[returnBitmap->m_info_head.biWidth * (biBitCount / 8) * row + column], sizeof(BYTE), 1, pf_rgb);
		}
		//跳过补位信息
		fseek(pf_rgb, sizeof(BYTE)*platoon_bit, SEEK_CUR);
	}
	cout << "位图解析成功！" << endl;
	return returnBitmap;
}
bool CBitmapHandle::SeparateRGB(CMyBitmap* originaLBmp, char* outputFileNameR, char* outputFileNameG, char* outputFileNameB) {

	cout << "位图三原色分离中，请稍后......" << endl;
	FILE *pf_r, *pf_g, *pf_b;
	pf_r = fopen(outputFileNameR, "wb");		//二进制读方式创建只有R的图像文件
	pf_g = fopen(outputFileNameG, "wb");		//二进制读方式创建只有G的图像文件
	pf_b = fopen(outputFileNameB, "wb");		//二进制读方式创建只有B的图像文件
	//写如文件头
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_r);
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_g);
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_b);
	//写入信息头
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_r);
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_g);
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_b);

	//写入实际的图像数据
	//位深度
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	BYTE zero = 0;
	for (int row = 0; row < originaLBmp->m_info_head.biHeight; row++) {
		for (int column = 0; column < originaLBmp->m_info_head.biWidth * 3; column += 3) {
			//写B色图像
			fwrite(&originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + column], sizeof(BYTE), 1, pf_b);
			fwrite(&zero, sizeof(BYTE), 1, pf_b);
			fwrite(&zero, sizeof(BYTE), 1, pf_b);
			//写G色图像
			fwrite(&zero, sizeof(BYTE), 1, pf_g);
			fwrite(&originaLBmp->m_factdata[row*originaLBmp->m_info_head.biWidth * 3 + column + 1], sizeof(BYTE), 1, pf_g);
			fwrite(&zero, sizeof(BYTE), 1, pf_g);
			//写R色图像
			fwrite(&zero, sizeof(BYTE), 1, pf_r);
			fwrite(&zero, sizeof(BYTE), 1, pf_r);
			fwrite(&originaLBmp->m_factdata[row*originaLBmp->m_info_head.biWidth * 3 + column + 2], sizeof(BYTE), 1, pf_r);
		}
		//补齐
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_b);
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_g);
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_r);
	}
	cout << "位图三原色分离成功！" << endl;
	//关闭打开的图像文件
	fclose(pf_r);
	fclose(pf_g);
	fclose(pf_b);
	return true;
	return true;
}
bool CBitmapHandle::GrayscaleBitmap(CMyBitmap* originaLBmp, char* outputFileName) {
	cout << "位图灰度化中，请稍后......" << endl;
	FILE *pf_gray;
	pf_gray = fopen(outputFileName, "wb");	//二进制写方式创建8位灰度图像文件
	CMyBitmap *targetBitmap = new CMyBitmap();
	//设置文件头
	targetBitmap->m_file_head = originaLBmp->m_file_head;
	//计算和设置从文件头到实际位图数据的偏移字节数
	DWORD targetBfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
	targetBitmap->m_file_head.bfOffBits = targetBfOffBits;
	//写入文件头
	fwrite(&targetBitmap->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_gray);

	//设置信息头
	targetBitmap->m_info_head = originaLBmp->m_info_head;
	targetBitmap->m_info_head.biBitCount = 8;
	targetBitmap->m_info_head.biSizeImage = (originaLBmp->m_info_head.biWidth * (targetBitmap->m_info_head.biBitCount / 8) + 3) / 4
											* 4 * originaLBmp->m_info_head.biHeight; 
	targetBitmap->m_info_head.biClrUsed = 256;
	targetBitmap->m_info_head.biClrImportant = 256;
	//写入信息头
	fwrite(&targetBitmap->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_gray);

	//设置和写入调色板
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

	//位深度
	int biBitCount = targetBitmap->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = targetBitmap->m_info_head.biSizeImage / targetBitmap->m_info_head.biHeight
		- targetBitmap->m_info_head.biWidth * (biBitCount / 8);
	//设置和写入位图实际数据
	targetBitmap->m_factdata = new BYTE[targetBitmap->m_info_head.biWidth * targetBitmap->m_info_head.biHeight];
	for (int row = 0; row < targetBitmap->m_info_head.biHeight; row++) {
		for (int column = 0; column < targetBitmap->m_info_head.biWidth; column++) {
			//计算灰度值
			BYTE gray_value =
					 (int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + 3 * column]) * 11
					+ int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + 3 * column + 1]) * 59
					+ int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth * 3 + 3 * column + 2]) * 30) / 100;
			//实际值存储
			targetBitmap->m_factdata[row * targetBitmap->m_info_head.biWidth + column] = gray_value;
			//写入灰色
			fwrite(&gray_value, sizeof(BYTE), 1, pf_gray);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)* platoon_bit, 1, pf_gray);
	}
	//关闭打开的图像文件
	fclose(pf_gray);

	return true;
}
bool CBitmapHandle::GrayscaleAntiColor(CMyBitmap* originaLBmp, char* outputFileName) {
	FILE *pf_gray;
	cout << "灰度位图反色中，请稍后......" << endl;
	pf_gray = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件
	//写入文件头
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_gray);
	//写入信息头
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_gray);
	//写入调色板
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_gray);
	}
	//位深度
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	for (int row = 0; row < originaLBmp->m_info_head.biHeight; row++) {
		for (int column = 0; column < originaLBmp->m_info_head.biWidth; column++) {
			//写入灰度图像
			BYTE grayValue = 255 - int(originaLBmp->m_factdata[row * originaLBmp->m_info_head.biWidth + column]);
			fwrite(&grayValue, sizeof(BYTE), 1, pf_gray);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_gray);
	}
	cout << "灰度位图反色成功！" << endl;
	//关闭打开的图像文件
	fclose(pf_gray);
	return true;
}
bool CBitmapHandle::Histogram(CMyBitmap* originaLBmp, char* outputFileName) {
	FILE *pf_bmp;
	int pBuffer[256];			//各灰度级的像素数量
	int maxGrayValue;			//直方图灰度最多像素数
	float heightCompress = 1.0;	//高度压缩倍数(有的灰度值得像素个数可能过多，图的高度就得增高，
								//为了将图片的高度保持在1000px之内，需要对其进行处理)
	int bitHeight = 250;		//直方图高度
	int bitWidth = 256;			//直方图宽度
	for (int i = 0; i < 256; i++) {
		pBuffer[i] = 0;			//各灰度像素点数置为0
	}
	cout << "图像直方图制作中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件
	//像素点总数量
	int pixelSum = int(originaLBmp->m_info_head.biWidth * originaLBmp->m_info_head.biHeight);
	//遍历所有像素点
	for (int i = 0; i< pixelSum; i++) {
		int grayValue = int(originaLBmp->m_factdata[i]);//当前像素点的灰度值
		pBuffer[grayValue]++;							//grayValue灰度的像素数量+1
	}
	//寻找最大高度
	maxGrayValue = 0;
	for (int i = 0; i < 256; i++) {
		maxGrayValue = pBuffer[i] > maxGrayValue ? pBuffer[i] : maxGrayValue;
	}
	//计算高度压缩比例
	heightCompress = float(maxGrayValue) / float(bitHeight);
	//重新计算各灰度像素总数量
	for (int i = 0; i < 256; i++) {
		pBuffer[i] = float(pBuffer[i]) / heightCompress;
	}
	//写入文件头信息
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
	//写入信息头信息
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
	//写入调色板
	RGBQUAD gbquad[2];
	//gbquad[0]为纯黑色，gbquad[1]为纯白色
	for (int i = 0; i < 2; i++) {
		gbquad[i].rgbBlue = i * 255;
		gbquad[i].rgbGreen = i * 255;
		gbquad[i].rgbRed = i * 255;
		gbquad[i].rgbReserved = i * 255;
		fwrite(&gbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//写入真实数据
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
	cout << "图像直方图制作成功！" << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);
	return true;
}
bool CBitmapHandle::HistogramEqualization(CMyBitmap* originaLBmp, char* outputFileName) {
	FILE *pf_bmp;
	int pxielNum[256];
	double pRobabilityFunc[256];//累计概率分布函数
	for (int i = 0; i < 256; i++) {
		pxielNum[i] = 0;
		pRobabilityFunc[i] = 0.0f;
	}

	cout << "灰度图均衡化中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件
	
	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
	//计算灰度像素点
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			int index = originaLBmp->m_factdata[row * bitWidth + column];
			pxielNum[index]++;
		}
	}
	//计算累计分布函数
	pRobabilityFunc[0] = double(pxielNum[0]) / (bitHeight* bitWidth);
	for (int i = 1; i < 256; i++) {
		pRobabilityFunc[i] = pRobabilityFunc[i-1]+ double(pxielNum[i]) / (bitHeight* bitWidth);
	}
	//输出图片
	//写入文件头
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//写入信息头
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//写入调色板
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//写入真实数据
	//位深度
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//写入灰度图像
			BYTE gbquadIndex = 255 * pRobabilityFunc[int(originaLBmp->m_factdata[row * bitWidth + column])];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}

	cout << "灰度图均衡化成功！" << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);
	return true;
}
