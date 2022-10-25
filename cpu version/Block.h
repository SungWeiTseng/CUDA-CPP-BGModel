#include<iostream>
#include"dataDef.h"

extern int BIN_NUM;
extern int BIN_WIDTH;
extern float THRESOLD;
extern int globCounter;
extern int lookUpTable[2][256];	//[0]index of bin [1]neer current bin index

class Block {
private:
	static const float riseRate;
	float beta = 0.95f;
	float alpha = 1.0 - beta;
	int curPixel;
	void cycleUpdate();
	void binDec(int);
	int* cPtr;
	float* bPtr;
	int locCounter[8];
	float Bin[8];

public:
	Block();
	~Block();
	void update(int);
	void LRAdjust(float);
	unsigned char getBackground();
	void init(int);
};
