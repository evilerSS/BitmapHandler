#include "CBitmapHandle.h"
#include "SplitStruct.h"
#include <Iostream>
#include <fstream>
#include <math.h>
#include<stack>

#define M_PI 3.1415926
#define M_ANGLE M_PI / 180
#define M_E  2.7182818
using namespace std;

//int CBitmapHandle::CDF(int n, float k)
//{ 
//	return 255 * k;
//}

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
	cout << "位图解析成功！" << endl << endl;
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
	cout << "位图三原色分离成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_r);
	fclose(pf_g);
	fclose(pf_b);
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
	cout << "灰度位图反色成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_gray);
	return true;
}
bool CBitmapHandle::Histogram(CMyBitmap* originaLBmp, char* outputFileName) {
	FILE *pf_bmp;
	int pBuffer[256] = {0};			//各灰度级的像素数量
	int maxGrayValue;			//直方图灰度最多像素数
	float heightCompress = 1.0f;//高度压缩倍数
	int bitHeight = 250;		//直方图高度
	int bitWidth = 256;			//直方图宽度
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
	cout << "图像直方图制作成功！" << endl << endl;
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
	cout << "灰度图均衡化成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);
	return true;
}


// 平均处理（基于模板卷积运算）（处理边界点）
bool CBitmapHandle::AverageHandleWithBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "灰度图降噪中(平均处理，包含边界)，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//写入灰度图像
			BYTE gbquadIndex;
			int filter[9];
			//计算平均值
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
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "灰度图降噪成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}

// 平均处理（基于模板卷积运算）（不处理边界点）
bool CBitmapHandle::AverageHandleWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "灰度图降噪中(平均处理，不包含边界)，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//写入灰度图像
			BYTE gbquadIndex;
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {//边界：不处理
				gbquadIndex = int(originaLBmp->m_factdata[row * bitWidth + column]);
			}
			else {//非边界：平均处理
				// 计算平均值
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
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "灰度图降噪成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}

// 中值滤波（处理边界点）
bool CBitmapHandle::MedianFilterWithBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "灰度图降噪中(中值滤波，包含边界)，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//写入灰度图像
			BYTE gbquadIndex;
			int filter[9];
			//计算平均值
			filter[0] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[1] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + column]);
			filter[2] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			filter[3] = int(originaLBmp->m_factdata[row * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[4] = int(originaLBmp->m_factdata[row * bitWidth + column]);
			filter[5] = int(originaLBmp->m_factdata[row * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			filter[6] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
			filter[7] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + column]);
			filter[8] = int(originaLBmp->m_factdata[((row - 1) < 0 ? 0 : (row - 1)) * bitWidth + ((column + 1) >= bitWidth ? column : (column + 1))]);
			//排序
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
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "灰度图降噪成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}

// 中值滤波（不处理边界点）
bool CBitmapHandle::MedianFilterWithoutBorder(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "灰度图降噪中(中值滤波，不包含边界)，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//写入灰度图像
			BYTE gbquadIndex;
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {//边界：不处理
				gbquadIndex = int(originaLBmp->m_factdata[row * bitWidth + column]);
			}
			else {//非边界：平均处理
				// 计算平均值
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
				//排序
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
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "灰度图降噪成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 图像缩放
bool CBitmapHandle::BitmapScale(CMyBitmap* originaLBmp, char* outputFileName, float x, float y)
{
	FILE *pf_bmp;
	cout << "图像缩放中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	//输出图片
	//写入文件头
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//写入信息头
	BITMAPINFOHEADER new_info_head = originaLBmp->m_info_head;
	new_info_head.biWidth = LONG(new_info_head.biWidth * x);
	new_info_head.biHeight = LONG(new_info_head.biHeight * y);
	new_info_head.biSizeImage = (new_info_head.biWidth * (new_info_head.biBitCount / 8) + 3) / 4 * 4 * new_info_head.biHeight;
	fwrite(&new_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//写入调色板
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		fwrite(&originaLBmp->m_rgbquad[i], sizeof(RGBQUAD), 1, pf_bmp);
	}
	//写入真实数据
	int biBitCount = new_info_head.biBitCount;
	//补位字节数
	int platoon_bit = new_info_head.biSizeImage / new_info_head.biHeight
		- new_info_head.biWidth * (biBitCount / 8);
	BYTE zero = 0;
	int bitHeight = new_info_head.biHeight;		//新图高度
	int bitWidth = new_info_head.biWidth;		//新图宽度
	//缩小
	float cX = 1 / x;
	float cY = 1 / y;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			BYTE gbquadIndex = originaLBmp->m_factdata[int(cY * row) * originaLBmp->m_info_head.biWidth + int(cX * column)];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}

	cout << "图像缩放成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 图像平移
bool CBitmapHandle::BitmapTranslation(CMyBitmap* originaLBmp, char* outputFileName, int x, int y)
{
	FILE *pf_bmp;
	cout << "图像平移中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
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
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像平移成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 图像镜像
bool CBitmapHandle::BitmapMirror(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "图像镜像图生成中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			BYTE gbquadIndex = originaLBmp->m_factdata[row * bitWidth + (bitWidth - column)];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像镜像图生成成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 图像旋转
bool CBitmapHandle::BitmapRotate(CMyBitmap* originaLBmp, char* outputFileName, int angle)
{
	FILE *pf_bmp;
	cout << "图像旋转中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
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
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像旋转成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 阈值分割：给定T
bool CBitmapHandle::ThresholdDivisionT(CMyBitmap* originaLBmp, char* outputFileName, int t)
{
	FILE *pf_bmp;
	cout << "图像阈值分割（给定的T为 "<< t <<" ）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	BYTE gbquadIndex;

	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {	
			if (originaLBmp->m_factdata[row * bitWidth + column] > t) {
				gbquadIndex = 255;
			}
			else {
				gbquadIndex = 0;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像阈值分割（给定T）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 阈值分割：迭代阈值法
bool CBitmapHandle::ThresholdDivisionIteration(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "图像阈值分割（迭代阈值法）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	int t = 128;		//初始化阈值
	int pGrayNum[256] = {0};	//各灰度级的像素数量
	//灰度密度函数统计
	//像素点总数量
	int pixelSum = int(originaLBmp->m_info_head.biWidth * originaLBmp->m_info_head.biHeight);
	//遍历所有像素点
	for (int i = 0; i < pixelSum; i++) {
		int grayValue = int(originaLBmp->m_factdata[i]);//当前像素点的灰度值
		pGrayNum[grayValue]++;							//grayValue灰度的像素数量+1
	}
	//迭代计算最佳阈值
	t = IterationT(pGrayNum, 128);
	cout << "最佳阈值为：" << t << endl;
	BYTE gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (originaLBmp->m_factdata[row * bitWidth + column] > t) {
				gbquadIndex = 255;
			}
			else {
				gbquadIndex = 0;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像阈值分割（迭代阈值法）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 阈值分割：Otsu
bool CBitmapHandle::ThresholdDivisionOtsu(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "图像阈值分割（Otsu法）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	int t = 0;				//最佳阈值
	float pGrayNum[256] = { 0 };//各灰度级的像素数量
	float wk[256] = { 0 };	//灰度级0到k的像素的出现概率
	float uk[256] = { 0 };	//平均灰度
	float variance = 0;		//类间方差
	float maxVariance = variance;//最大类间方差
	//像素点总数量
	int pixelSum = int(originaLBmp->m_info_head.biWidth * originaLBmp->m_info_head.biHeight);

	//遍历所有像素点
	for (int i = 0; i < pixelSum; i++) {
		int grayValue = int(originaLBmp->m_factdata[i]);//当前像素点的灰度值
		pGrayNum[grayValue]++;							//grayValue灰度的像素数量+1
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
		if (variance > maxVariance) {//如果当前方差大于最大方差
			maxVariance = variance;	//更新最大方差
			t = i;					//更新最佳阈值
		}
	}
	//阈值方差计算
	cout << "最佳阈值为：" << t << endl;
	BYTE gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (originaLBmp->m_factdata[row * bitWidth + column] > t) {
				gbquadIndex = 255;
			}
			else {
				gbquadIndex = 0;
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像阈值分割（Otsu法）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 阈值计算迭代函数
int CBitmapHandle::IterationT(int *grayNum, int oldT)
{
	int newT = 0;
	int u1 = 0, u2 = 0;
	int numerator = 0, denominator = 0;
	for (int i = 0; i < oldT; i++)
	{	
		numerator += i * grayNum[i];
		denominator += grayNum[i];
	}
	u1 = numerator / denominator;
	numerator = 0;
	denominator = 0;
	for (int i = oldT; i < 255; i++)
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



// 基于种子点进行区域增长
bool CBitmapHandle::RegionGrowingWithSeed(CMyBitmap* originaLBmp, char* outputFileName, POINT seedPoint, int t)
{
	FILE *pf_bmp;
	cout << "图像分割（基于种子点进行区域增长）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	BYTE gbquadIndex;
	stack<POINT> seeds;
	POINT point = seedPoint,
		temppoint = seedPoint;//临时存放种子的中间变量
	int *pixel = new int[bitWidth * bitHeight];
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			pixel[row * bitWidth + column] = 0;
		}
	}
	//种子点像素值设置
	pixel[seedPoint.y * bitWidth + seedPoint.x] = 255;
	// 基于种子点的区域增长开始
	//cout << seedPoint.x << "," << seedPoint.y << "," << int(originaLBmp->m_factdata[(point.y + 1) * bitWidth + (point.x - 1)]) << endl;
	seeds.push(seedPoint);
	while (!seeds.empty())
	{
		point = seeds.top();//返回栈顶元素，不删除
		seeds.pop();//删除栈顶元素
		//cout << point.x << "," << point.y << ";  ";
		if (point.x > 0 && point.x < bitWidth - 1 && point.y > 0 && point.y < bitHeight - 1) {//是否为非边界像素
			int currentGray  = int(originaLBmp->m_factdata[point.x * bitWidth + point.y]);
			//cout << currentGray << ";  ";
			int index;
			//左上
			index = (point.y + 1) * bitWidth + (point.x - 1);
			//cout << "当前像素灰度：" << currentGray << ";  " << "左上角："<<int(originaLBmp->m_factdata[index])<< endl;
			if (pixel[index] == 0
				&& 
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
				//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x - 1;
				temppoint.y = point.y + 1;
				seeds.push(temppoint);
			}
			//上
			index = (point.y + 1) * bitWidth + (point.x);
			if (pixel[index] == 0
				&&
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
				//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x;
				temppoint.y = point.y + 1;
				seeds.push(temppoint);
			}
			//右上
			index = (point.y + 1) * bitWidth + (point.x + 1);
			if (pixel[index] == 0
				&&
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
				//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x + 1;
				temppoint.y = point.y + 1;
				seeds.push(temppoint);
			}
			//左
			index = (point.y) * bitWidth + (point.x - 1);
			if (pixel[index] == 0
				&&
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
				//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x - 1;
				temppoint.y = point.y;
				seeds.push(temppoint);
			}
			//右
			index = (point.y) * bitWidth + (point.x + 1);
			if (pixel[index] == 0
				&&
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
				//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x + 1;
				temppoint.y = point.y ;
				seeds.push(temppoint);
			}
			//左下
			index = (point.y - 1) * bitWidth + (point.x - 1);
			if (pixel[index] == 0
				&&
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
				//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x - 1;
				temppoint.y = point.y - 1;
				seeds.push(temppoint);
			//下
			index = (point.y - 1) * bitWidth + (point.x);
			if (pixel[index] == 0
				&&
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
					//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x;
				temppoint.y = point.y - 1;
				seeds.push(temppoint);
				}
			}
			//右下
			index = (point.y - 1) * bitWidth + (point.x + 1);
			if (pixel[index] == 0
				&&
				abs(int(originaLBmp->m_factdata[index]) - currentGray) < t) {
				//如果满足增长条件
				pixel[index] = 255;
				temppoint.x = point.x + 1;
				temppoint.y = point.y - 1;
				seeds.push(temppoint);
			}
		}
	}

	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			gbquadIndex = pixel[row * bitWidth + column];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像分割（基于种子点进行区域增长）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 无种子点的区域增长
bool CBitmapHandle::RegionGrowingWithoutSeed(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "图像分割（分裂合并）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	stack<SplitStruct> stack;		//存储需要分割的区域
	SplitStruct splitStruct, splitStructTemp;//每次余姚处理的SplitStruct和临时SplitStruct
	//初始化需要分割的图像
	splitStruct.nWidth = bitWidth;
	splitStruct.nHeight = bitHeight;
	splitStruct.nOffsetWidth = 0;
	splitStruct.nOffsetHeight = 0;
	stack.push(splitStruct);
	//用于存储图像的属性值（=该块图像的所有像素灰度值之和除以该块图像所有像素点的数量，即平均灰度值）
	int nValues[2][2];
	int nWidthTemp[3], nHeightTemp[3], nTemp;
	int nWidth, nHeight;
	int n, m, l;
	double dOver;
	BYTE* targetBmpData = new BYTE[bitWidth * bitHeight];
	while (!stack.empty()) {
		//取出栈顶元素并将栈顶元素弹出栈
		splitStruct = stack.top();
		stack.pop();

		n = splitStruct.nOffsetHeight * bitWidth + splitStruct.nOffsetWidth;	//该图像的左下角
		//把图像分成 2*2块
		nWidthTemp[0] = 0;
		nWidthTemp[2] = (splitStruct.nWidth + 1) / 2;//左边的宽度
		nWidthTemp[1] = splitStruct.nWidth - nWidthTemp[2];//右边的宽度

		nHeightTemp[0] = 0;
		nHeightTemp[2] = (splitStruct.nHeight + 1) / 2;//下面的高度
		nHeightTemp[1] = splitStruct.nHeight - nHeightTemp[2];//上面的高度
		//计算每一块图像的属性值
		int nValue;		//记录某个区域的灰度值和
		int nValueTemp;	//记录某个像素的灰度值
		for (int i = 1; i < 3; i++) {
			for (int j = 1; j < 3; j++) {
				//遍历左下、左上、右下、右上四个区域
				nValue = 0;
				m = n + bitWidth * nHeightTemp[i - 1] + nWidthTemp[j - 1];
				for (nHeight = 0; nHeight < nHeightTemp[i]; nHeight++) {
					for (nWidth = 0; nWidth < nWidthTemp[j]; nWidth++) {
						l = m + bitWidth * nHeight + nWidth;
						nValueTemp = int(originaLBmp->m_factdata[l]);
						nValue += nValueTemp;
					}
				}
				if (nHeightTemp[i] * nWidthTemp[j] == 0)
				{
					continue;
				}
				if (nHeightTemp[i] * nWidthTemp[j] == 1) {
					if (int(originaLBmp->m_factdata[l]) < 150) {
						originaLBmp->m_factdata[l] = 0;
					}
					else {
						originaLBmp->m_factdata[l] = 255;
					}
					continue;
				}
				//各块图像的灰度平均值
				nValues[i - 1][j - 1] = nValue / (nHeightTemp[i] * nWidthTemp[j]);
				//对每一块进行判断是否继续分裂
				if (nValues[i - 1][j - 1] < 280) {//平均灰度值少于200需要继续分裂（分裂准则）
					splitStructTemp.nWidth = nWidthTemp[j];
					splitStructTemp.nHeight = nHeightTemp[i];
					splitStructTemp.nOffsetWidth = splitStruct.nOffsetWidth + nWidthTemp[j - 1];
					splitStructTemp.nOffsetHeight = splitStruct.nOffsetHeight + nHeightTemp[i - 1];
					stack.push(splitStructTemp);
				}
				else {//合并（直接填充该图像为黑色）
					for (nHeight = 0; nHeight < nHeightTemp[i]; nHeight++)
					{
						for (nWidth = 0; nWidth < nWidthTemp[j]; nWidth++)
						{
							l = m + bitWidth * nHeight + nWidth;
							originaLBmp->m_factdata[l] = 255;
						}
					}
				}

			}
		}
	}

	int gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			gbquadIndex = (int)originaLBmp->m_factdata[row * bitWidth + column];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像分割（分裂合并）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}



// 边缘检测：Prewitt
bool CBitmapHandle::EdgeDetectionPrewitt(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "图像边缘检测（Prewitt）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	BYTE gbquadIndex;
	int fz[9];
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {
				gbquadIndex = 0;
			}
			else {
				//计算邻域
				fz[0] = originaLBmp->m_factdata[(row + 1) * bitWidth + (column - 1)];
				fz[1] = originaLBmp->m_factdata[(row + 1) * bitWidth + (column)];
				fz[2] = originaLBmp->m_factdata[(row + 1) * bitWidth + (column + 1)];
				fz[3] = originaLBmp->m_factdata[(row) * bitWidth + (column - 1)];
				fz[4] = originaLBmp->m_factdata[(row) * bitWidth + (column)];
				fz[5] = originaLBmp->m_factdata[(row) * bitWidth + (column + 1)];
				fz[6] = originaLBmp->m_factdata[(row - 1) * bitWidth + (column - 1)];
				fz[7] = originaLBmp->m_factdata[(row - 1) * bitWidth + (column)];
				fz[8] = originaLBmp->m_factdata[(row - 1) * bitWidth + (column + 1)];
				int Gx = (fz[6] + fz[7] + fz[8]) - (fz[0] + fz[1] + fz[2]);
				int Gy = (fz[2] + fz[5] + fz[8]) - (fz[0] + fz[3] + fz[6]);
				int G = abs(Gx) + abs(Gy);
				
				if (G > 130) {
					gbquadIndex = 255;
				}
				else {
					gbquadIndex = 0;
				}
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像边缘检测（Prewitt）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 边缘检测：Sobel
bool CBitmapHandle::EdgeDetectionSobel(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "图像边缘检测（Sobel）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	BYTE gbquadIndex;
	int fz[9];
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {
				gbquadIndex = 0;
			}
			else {
				//计算邻域
				fz[0] = originaLBmp->m_factdata[(row + 1) * bitWidth + (column - 1)];
				fz[1] = originaLBmp->m_factdata[(row + 1) * bitWidth + (column)];
				fz[2] = originaLBmp->m_factdata[(row + 1) * bitWidth + (column + 1)];
				fz[3] = originaLBmp->m_factdata[(row)* bitWidth + (column - 1)];
				fz[4] = originaLBmp->m_factdata[(row)* bitWidth + (column)];
				fz[5] = originaLBmp->m_factdata[(row)* bitWidth + (column + 1)];
				fz[6] = originaLBmp->m_factdata[(row - 1) * bitWidth + (column - 1)];
				fz[7] = originaLBmp->m_factdata[(row - 1) * bitWidth + (column)];
				fz[8] = originaLBmp->m_factdata[(row - 1) * bitWidth + (column + 1)];
				int Gx = (fz[6] + 2 * fz[7] + fz[8]) - (fz[0] + 2 * fz[1] + fz[2]);
				int Gy = (fz[2] + 2 * fz[5] + fz[8]) - (fz[0] + 2 * fz[3] + fz[6]);
				int G = abs(Gx) + abs(Gy);

				if (G > 130) {
					gbquadIndex = 255;
				}
				else {
					gbquadIndex = 0;
				}
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像边缘检测（Sobel）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 边缘检测：LOG
bool CBitmapHandle::EdgeDetectionLOG(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "图像边缘检测（LOG）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	BYTE gbquadIndex;
	int fz[16];
	float t = 1.0;
	BYTE* targetDate = new BYTE[bitWidth * bitHeight];
	//平滑处理(高斯滤波器)
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			//写入灰度图像
			BYTE gbquadIndex;
			int filter[9];
			//计算平均值
			filter[0] = int(originaLBmp->m_factdata[((row + 1) >= bitWidth ? row : (row + 1)) * bitWidth + ((column - 1) < 0 ? column : (column - 1))]);
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
			targetDate[row * bitWidth + column] = result;
		}
	}
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {
				gbquadIndex = 0;
			}
			else {
				//计算邻域
				fz[0] = targetDate[(row + 1) * bitWidth + (column - 1)];
				fz[1] = targetDate[(row + 1) * bitWidth + (column)];
				fz[2] = targetDate[(row + 1) * bitWidth + (column + 1)];
				fz[3] = targetDate[(row)* bitWidth + (column - 1)];
				fz[4] = targetDate[(row)* bitWidth + (column)];
				fz[5] = targetDate[(row)* bitWidth + (column + 1)];
				fz[6] = targetDate[(row - 1) * bitWidth + (column - 1)];
				fz[7] = targetDate[(row - 1) * bitWidth + (column)];
				int G = abs(4 * fz[4] - (fz[1] + fz[3] + fz[5] + fz[7]));
				if (G * G > 130) {
					gbquadIndex = 255;
				}
				else {
					gbquadIndex = 0;
				}
			}
			//gbquadIndex = targetDate[row * bitWidth + column];
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "图像边缘检测（LOG）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 直线检测：Hough变换
bool CBitmapHandle::LineDetectionHough(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "直线检测（Hough）中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);
	//柔
	//rho取值
	//计算图像对角线长度
	int angleNum = 180;
	int length = sqrt(bitWidth * bitWidth + bitHeight * bitHeight);
	int* theta = new  int[angleNum];
	int* A = new int[angleNum * length * 2];
	stack<int> lineAIndexStack;
	int lineAIndexLength = 0;
	//theta取值：0-179，每隔1度取一次
	//p = x * cos0 + y * sin 0
	for (int i = 0; i < angleNum; i++)
	{
		theta[i] = i;
	}
	for (int i = 0; i < angleNum * length * 2; i++)
	{
		A[i] = 0;
	}
	//求A(theta, rho)
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++)
		{
			if (int(originaLBmp->m_factdata[row * bitWidth + column]) == 255) {
				for (int i = 0; i < angleNum; i++)
				{
					int rho = int(column * cos(theta[i] * M_ANGLE) + row * sin(theta[i] * M_ANGLE)) % length;
					rho = rho + length;
					A[rho * angleNum + i] ++;
				}
			}
		}
	}
	//求可组成直线的(theta, rho)在A中的索引值
	for (int y = 0; y < length * 2; y++)
	{
		for (int x = 0; x < angleNum; x++)
		{
			if (A[y * angleNum + x] >= 20) {
				lineAIndexStack.push(y * angleNum + x);
				lineAIndexLength++;
			}
		}
	}
	//栈转化为数组
	int* lineAIndexArray = new int[lineAIndexLength];
	int i = 0;
	while (!lineAIndexStack.empty()) {
		lineAIndexArray[i] = lineAIndexStack.top();
		lineAIndexStack.pop();
		i++;
	}
	BYTE gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			gbquadIndex = 0;
			//判断这个像素点是否在某条直线上面
			for (int i = 0; i < lineAIndexLength; i++)
			{
				int t = lineAIndexArray[i] % angleNum;//theta
				int r = lineAIndexArray[i] / angleNum - length;//rho
				if (abs(column * cos(t * M_ANGLE) + row * sin(t * M_ANGLE) - r) <= 0.0001) {
					gbquadIndex = 255;
				}
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "直线检测（Hough）成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 连通域分析
bool CBitmapHandle::ConnectedDomainAnalysis(CMyBitmap* originaLBmp, char* outputFileName)
{
	FILE *pf_bmp;
	cout << "连通域分析中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
	//输出图片
	//写入文件头
	fwrite(&originaLBmp->m_file_head, sizeof(BITMAPFILEHEADER), 1, pf_bmp);
	//写入信息头
	fwrite(&originaLBmp->m_info_head, sizeof(BITMAPINFOHEADER), 1, pf_bmp);
	//写入调色板
	for (int i = 0; i < originaLBmp->m_info_head.biClrUsed; i++)
	{
		RGBQUAD rgb;
		if (i % 3 == 0) {
			rgb.rgbRed = i * 0.5;
			rgb.rgbGreen = i * 0.35;
			rgb.rgbBlue = i * 0.15;
		}
		if (i % 3 == 1) {
			rgb.rgbRed = i * 0.35;
			rgb.rgbGreen = i * 0.15;
			rgb.rgbBlue = i * 0.5;
		}
		if (i % 3 == 2) {
			rgb.rgbRed = i * 0.15;
			rgb.rgbGreen = i * 0.5;
			rgb.rgbBlue = i * 0.35;
		}
		fwrite(&rgb, sizeof(RGBQUAD), 1, pf_bmp);
	}
	//写入真实数据
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	int remarkNum = 0;
	int* mark = new int[bitWidth * bitHeight];
	stack<POINT> point_stack;
	POINT origin;
	for (int i = 0; i < bitWidth * bitHeight; i++)
	{
		mark[i] = -1;//尚未标记
	}
	for (int row = 1; row < bitHeight - 1; row++) {
		for (int column = 1; column < bitWidth - 1; column++) {
			if (mark[row * bitWidth + column] == -1 && int(originaLBmp->m_factdata[row * bitWidth + column] == 0)) {
				//尚未标记并且为目标灰度
				mark[row * bitWidth + column] = remarkNum;//标记该点
				remarkNum++;//标记加1
				POINT neiPoint[8];
				POINT p;
				p.x = column;
				p.y = row;
				point_stack.push(p);
				while (!point_stack.empty())
				{
					POINT currPt = point_stack.top();
					point_stack.pop();
					origin.x = currPt.x;
					origin.y = currPt.y;
					//邻域
					neiPoint[0].x = currPt.x - 1;
					neiPoint[0].y = currPt.y + 1;
					neiPoint[1].x = currPt.x;
					neiPoint[1].y = currPt.y + 1;
					neiPoint[2].x = currPt.x + 1;
					neiPoint[2].y = currPt.y + 1;

					neiPoint[3].x = currPt.x - 1;
					neiPoint[3].y = currPt.y;
					neiPoint[4].x = currPt.x + 1;
					neiPoint[4].y = currPt.y;

					neiPoint[5].x = currPt.x - 1;
					neiPoint[5].y = currPt.y - 1;
					neiPoint[6].x = currPt.x;
					neiPoint[6].y = currPt.y - 1;
					neiPoint[7].x = currPt.x + 1;
					neiPoint[7].y = currPt.y - 1;
					for (int i = 0; i < 8; i++) {
						if (neiPoint[i].y < 0 || neiPoint[i].y > bitHeight - 1 || neiPoint[i].x < 0 || neiPoint[i].x > bitWidth - 1) {
							continue;
						}
						else {
							if (mark[neiPoint[i].y * bitWidth + neiPoint[i].x] != -1) {
								continue;
							}
							if (int(originaLBmp->m_factdata[neiPoint[i].y * bitWidth + neiPoint[i].x]) == 0) {
								point_stack.push(neiPoint[i]);
								mark[neiPoint[i].y * bitWidth + neiPoint[i].x] = mark[origin.y * bitWidth + origin.x];
							}
						}
					
					}
				}
			}
		}
	}
	cout << remarkNum << endl;
	int *remarkGray = new int[remarkNum];
	//根据流通域个数将0-255灰度值分割,背景为白色
	for (int i = 0; i < remarkNum; i++)
	{
		remarkGray[i] = i * 255 / remarkNum;
	}
	BYTE gbquadIndex;
	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (mark[row * bitWidth + column] == -1) {
				gbquadIndex = 255;
			}
			else {
				gbquadIndex = remarkGray[mark[row * bitWidth + column]];
			}
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "连通域分析成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}


// 轮廓提取
bool CBitmapHandle::OutlineExtract(CMyBitmap* originaLBmp, char* outputFileName)
{

	FILE *pf_bmp;
	cout << "轮廓提取中，请稍后......" << endl;
	pf_bmp = fopen(outputFileName, "wb");//二进制读方式创建8位灰度图像文件

	int bitHeight = originaLBmp->m_info_head.biHeight;		//原图高度
	int bitWidth = originaLBmp->m_info_head.biWidth;		//原图宽度
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
	int biBitCount = originaLBmp->m_info_head.biBitCount;
	//补位字节数
	int platoon_bit = originaLBmp->m_info_head.biSizeImage / originaLBmp->m_info_head.biHeight
		- originaLBmp->m_info_head.biWidth * (biBitCount / 8);

	BYTE* targetDate = new BYTE[bitWidth * bitHeight];
	BYTE gbquadIndex;
	int colorGray[8];
	//遍历像素掏空目标区域的内部点

	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			if (row == 0 || row == bitHeight - 1 || column == 0 || column == bitWidth - 1) {
				targetDate[row * bitWidth + column] = 255;
				continue;
			}
			//判断是否为背景色（黑色）
			if (int(originaLBmp->m_factdata[row * bitWidth + column]) == 0) {
				targetDate[row * bitWidth + column] = 255;
				//判断其领域是否都是白色
				colorGray[0] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + (column - 1)]);
				colorGray[1] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + (column)]);
				colorGray[2] = int(originaLBmp->m_factdata[(row + 1) * bitWidth + (column + 1)]);
				colorGray[3] = int(originaLBmp->m_factdata[(row)* bitWidth + (column - 1)]);
				colorGray[4] = int(originaLBmp->m_factdata[(row)* bitWidth + (column + 1)]);
				colorGray[5] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + (column - 1)]);
				colorGray[6] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + (column)]);
				colorGray[7] = int(originaLBmp->m_factdata[(row - 1) * bitWidth + (column + 1)]);
				for (int i = 0; i < 8; i++) {
					if (int(colorGray[i]) != 0){
						targetDate[row * bitWidth + column] = 0;
						break;
					}
				}
			}
			else {
				targetDate[row * bitWidth + column] = 255;
			}
		}
	}

	for (int row = 0; row < bitHeight; row++) {
		for (int column = 0; column < bitWidth; column++) {
			gbquadIndex = int(targetDate[row * bitWidth + column]);
			fwrite(&gbquadIndex, sizeof(BYTE), 1, pf_bmp);
		}
		//补齐
		BYTE zero = 0;
		fwrite(&zero, sizeof(BYTE)*platoon_bit, 1, pf_bmp);
	}
	cout << "轮廓提取成功！" << endl << endl;
	//关闭打开的图像文件
	fclose(pf_bmp);

	return true;
}
