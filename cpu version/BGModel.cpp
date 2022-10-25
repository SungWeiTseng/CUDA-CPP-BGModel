#include"BGModel.h"
#include<opencv.hpp>

using namespace cv;

int BIN_NUM = 32;
int BIN_WIDTH = 256 / BIN_NUM;
float THRESOLD = 0.35f;
int BLOCK_WIDTH = 4;
int BLOCK_HEIGHT = BLOCK_WIDTH;
int IMG_SIZE_X = 640;
int IMG_SIZE_Y = 480;
int globCounter = 0;
int lookUpTable[2][256] { 0 };
int totalBlock;


BGModel::BGModel(int bNum, int imgW, int imgH) {
	BIN_NUM = bNum;
	BIN_WIDTH = 256 / BIN_NUM;
	IMG_SIZE_X = imgW;
	IMG_SIZE_Y = imgH;
	BLOCK_WIDTH = 1;
	BLOCK_HEIGHT = BLOCK_WIDTH;
	BLOCK_SIZE_X = IMG_SIZE_X / BLOCK_WIDTH;
	BLOCK_SIZE_Y = IMG_SIZE_Y / BLOCK_HEIGHT;
	totalBlock = BLOCK_SIZE_X * BLOCK_SIZE_Y;
	//foregroundMap = new bool[IMG_SIZE_X*IMG_SIZE_Y];
	blocks = new Block[totalBlock];
	fgPtr = &foregroundMap[0];
	blkPtr = &blocks[0];
	for (int j = 0; j < 256; j++) {
		lookUpTable[0][j] = j / BIN_WIDTH;
		lookUpTable[1][j] = j - lookUpTable[0][j] * BIN_WIDTH >= BIN_WIDTH / 2 ?
			lookUpTable[0][j] + 1 : lookUpTable[0][j] - 1;
		/*if (j - lookUpTable[0][j] * BIN_WIDTH >= BIN_WIDTH / 2) {
			if (lookUpTable[0][j] + 1 < BIN_NUM)lookUpTable[1][j] = lookUpTable[0][j] + 1;
		}
		else {
			if (lookUpTable[0][j] - 1 >= 0)lookUpTable[1][j] = lookUpTable[0][j] - 1;
		}*/
	}
};

BGModel::~BGModel() {
	free();
}

unsigned char* BGModel::getBG() {
	/*for (int y = 0; y < IMG_SIZE_Y; y++) {
		for (int x = 0; x < IMG_SIZE_X; x++) {
			int idx = (y / BLOCK_WIDTH)*BLOCK_SIZE_X + x / BLOCK_WIDTH;
			foregroundMap[y*IMG_SIZE_X + x] = blocks[idx].getBackground();
		}
	}*/
	/*for (int i = 0; i < totalBlock; i++) {
		foregroundMap[i] = blocks[i].getBackground();
	}*/
	for (unsigned char *fgPtr = foregroundMap, *lim = foregroundMap + totalBlock; fgPtr < lim; fgPtr++, blkPtr++)
		*fgPtr = blkPtr->getBackground();
	blkPtr = blocks;
	return foregroundMap;
}

void BGModel::LRAdjust(unsigned char* LRMap) {
	for (int i = 0; i < totalBlock; i++)
		blocks[i].LRAdjust(LRMap[i]);
}

void BGModel::input(unsigned char* img) {
	globCounter = (globCounter + 1) & (BIN_NUM - 1);
	/*for (int y = 0; y < IMG_SIZE_Y; y++) {
		for (int x = 0; x < IMG_SIZE_X; x++) {
			int idx = (y / BLOCK_WIDTH)*BLOCK_SIZE_X + x / BLOCK_WIDTH;
			blocks[idx].update(img[y*IMG_SIZE_X + x]);
		}
	}*/
	for (Block* blkPtr = blocks, *lim = blocks + totalBlock; blkPtr < lim; blkPtr++) {
		blkPtr->update(*(img++));
	}
}

void BGModel::free() {
	if (blocks != NULL) {
		delete[] blocks;
		blocks = NULL;
	}
	/*if (foregroundMap != NULL) {
		delete[] foregroundMap;
		foregroundMap = NULL;
	}*/
	fgPtr = NULL;
	blkPtr = NULL;
}

void BGModel::setThr(float t) {
	THRESOLD = t;
}

void BGModel::init(unsigned char* img) {
	/*for (int y = 0; y < IMG_SIZE_Y; y++) {
		for (int x = 0; x < IMG_SIZE_X; x++) {
			int idx = (y / BLOCK_WIDTH)*BLOCK_SIZE_X + x / BLOCK_WIDTH;
			blocks[idx].init(img[y*IMG_SIZE_X + x]);
		}
	}*/
	for (Block* blkPtr = blocks, *lim = blocks + totalBlock; blkPtr < lim; blkPtr++) {
		blkPtr->init(*(img++));
	}
}
//void BGModel::imgResize(Mat img) {
//	Size dsize = Size(640, 480);
//	Mat image2 = Mat(dsize, CV_8S);
//	resize(img, image2, dsize);
//}

