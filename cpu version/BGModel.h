#include<iostream>
#include"Block.h"
#include<highgui.h>

class BGModel {
private:
	int BLOCK_SIZE_X;
	int BLOCK_SIZE_Y;
	Block* blkPtr;
	Block* blocks;
	unsigned char foregroundMap[640*480];
	unsigned char* fgPtr;
public:
	BGModel(int bNum, int imgW, int imgH);
	~BGModel();
	unsigned char* getBG();
	void input(unsigned char*);
	void free();
	void setThr(float);
	void LRAdjust(unsigned char*);
	void init(unsigned char*);
	//void imgResize(Mat);
};