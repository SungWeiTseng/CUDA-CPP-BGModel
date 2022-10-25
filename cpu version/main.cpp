#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <fstream>
#include "opencv.hpp"
#include"BGModel.h"

using namespace std;
using namespace cv;

int main() {

	int imgW = 640;
	int imgH = 480;
	BGModel bgmodel(8, imgW, imgH);
	
	int count = 1;
	char fullName[1024] = "";
	char saveName[100] = "";
	sprintf(fullName, "D:\\CoVision\\VideoSurvei\\科技部計畫\\highway\\input\\in00%04d.jpg", count);
	Mat srcImg = imread(fullName, 0);
	Size dsize = Size(imgW, imgH);
	Mat desImg = Mat(dsize, CV_8S);
	Mat fgImg = Mat(dsize, CV_8S);
	resize(srcImg, desImg, dsize);
	resize(srcImg, fgImg, dsize);
	uchar* data = desImg.data;
	bgmodel.init(data);
	double START, END;
	START = clock();
	while (count<1000) {
		count++;
		sprintf(fullName, "D:\\CoVision\\VideoSurvei\\科技部計畫\\highway\\input\\in00%04d.jpg", count);
		srcImg = imread(fullName, 0);
		resize(srcImg, desImg, dsize);
		imshow("desImg", desImg);
		cvWaitKey(1);
		data = desImg.data;
		bgmodel.input(data);
		fgImg.data = bgmodel.getBG();
		imshow("fgImg", fgImg);
		cvWaitKey(1);
	}
	END = clock();
	cout << (END - START) / CLOCKS_PER_SEC << endl;
	cout << (float)count / ((END - START) / CLOCKS_PER_SEC) << endl;
	system("pause");
	return 0;
}