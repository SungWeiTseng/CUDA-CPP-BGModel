#include "Block.h"
#include <time.h>
using namespace std;

float power(float base, int e) {
	if (e == 0) return 1;
	if (e == 1) return base;
	float r = base;
	for (int i = 0; i < e - 1; i++) {
		r *= base;
	}
	return r;
}
const float Block::riseRate = 1;
Block::Block() :curPixel(0)
{	
	memset(Bin, 0, sizeof(Bin));
	memset(locCounter, 0, sizeof(locCounter));
	bPtr = &Bin[0];
	cPtr = &locCounter[0];
	/*locCounter = new int[BIN_NUM] {0};
	Bin = new float[BIN_NUM] {0};*/
}

Block::~Block() {
	/*if (locCounter != NULL) {
		delete[] locCounter;
		locCounter = NULL;
	}
	if (Bin != NULL) {
		delete[] Bin;
		Bin = NULL;
	}*/
	bPtr = NULL;
	cPtr = NULL;
}

void Block::init(int intensity) {
	int k0 = lookUpTable[0][intensity];
	int k1 = lookUpTable[1][intensity];
	if (k1 >= 0 && k1 < BIN_NUM) {
		Bin[k0] = 0.66f;
		Bin[k1] = 0.33f;
	}
	else {
		Bin[k0] = 1;
	}
}

void Block::binDec(int idx) {
	if (idx != globCounter) {
		int d = globCounter == locCounter[idx] ?
			BIN_NUM : (globCounter - locCounter[idx] + BIN_NUM) & (BIN_NUM - 1);
		Bin[idx] *= power(beta, d);
		locCounter[idx] = globCounter;
	}
}

void Block::cycleUpdate() {
	int d = globCounter == locCounter[globCounter] ?
		BIN_NUM : (globCounter - locCounter[globCounter] + BIN_NUM) & (BIN_NUM - 1);
	Bin[globCounter] *= power(beta, d);
	locCounter[globCounter] = globCounter;
}

void Block::update(int intensity) {
	int k0 = lookUpTable[0][intensity];
	int k1 = lookUpTable[1][intensity];
	curPixel = intensity;
	cycleUpdate();
	if (k1 >= 0 && k1 < BIN_NUM) {
		binDec(k0);
		binDec(k1);
		Bin[k0] += (alpha * 2 / 3);
		Bin[k1] += (alpha / 3);
	}
	else {
		binDec(k0);
		Bin[k0] += alpha;
	}
}

void Block::LRAdjust(float prob) {
	alpha = riseRate * prob;
	beta = 1 - alpha;
}

unsigned char Block::getBackground() {
	if (Bin[lookUpTable[0][curPixel]] > THRESOLD)return 0;
	if (lookUpTable[1][curPixel] >= 0 && lookUpTable[1][curPixel] < BIN_NUM)
		if (Bin[lookUpTable[1][curPixel]] > THRESOLD)return 0;
	return 255;
}
