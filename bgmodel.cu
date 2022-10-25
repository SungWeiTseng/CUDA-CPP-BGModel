#include "bgmodel.h"
#include<iostream>
#include<cv.h>
#include<cstdio>
#include<highgui.h>
#include<windows.h>  
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_functions.h"

using namespace cv;
using namespace std;


__global__ void Update(unsigned char* inputImg, unsigned char* outputImg, float* Probability)
{

	int idx = blockIdx.x * (BLOCKS_WID * BLOCKS_HEI) * 256 + (blockIdx.z * BLOCKS_WID + blockIdx.y) * 256 + threadIdx.y * blockDim.x + threadIdx.x;
	//n*(40*30) * 256 + (blk_y * 40 + blk_x) * 256 + threadIdx.y * 16 + threadIdx.x;
	int binIDX = idx * BIN_NUM;
	int currentBin = inputImg[idx] / (DELTA_B);
	if (Probability[binIDX + currentBin] < THRESHOLD) outputImg[idx / 3] = 255;
	for (int bin = 0; bin < BIN_NUM; bin++) {
		Probability[binIDX + bin] *= (BETA);
	}
	Probability[binIDX + currentBin] += ALPHA;
}

__global__ void Init(unsigned char* inputImg, float* Probability)
{
	int idx = blockIdx.x * (BLOCKS_WID * BLOCKS_HEI) * 256 + (blockIdx.z * BLOCKS_WID + blockIdx.y) * 256 + threadIdx.y * blockDim.x + threadIdx.x;
	int binIDX = idx * BIN_NUM;
	int currentBin = inputImg[idx] / (DELTA_B);
	Probability[binIDX + currentBin] = 1.0f;
}

int main(void)
{
	dim3 dimBlock(THREADS_PER_BLOCK, THREADS_PER_BLOCK);
	dim3 dimGrid(3, BLOCKS_WID, BLOCKS_HEI);

	IplImage* InputImg;
	IplImage* resizeImg;
	IplImage* OutputImg;

	cudaSetDevice(1);

	OutputImg = cvCreateImage(cvSize(PIX_WID, PIX_HEI), 8, 1);
	resizeImg = cvCreateImage(cvSize(PIX_WID, PIX_HEI), 8, 3);
	unsigned char* imgBuffer;
	unsigned char* outBuffer;

	unsigned char* cudaInputBuffer;
	unsigned char* cudaOutputBuffer;
	float* cudaProbability;
	outBuffer = (unsigned char*)OutputImg->imageData;
	cudaMalloc((void**)&cudaInputBuffer, TOTAL_PIX * 3);
	cudaMalloc((void**)&cudaOutputBuffer, TOTAL_PIX);
	cudaMalloc((void**)&cudaProbability, BIN_NUM * TOTAL_PIX * 3 * sizeof(float));
	cudaMemset(cudaProbability, 0, BIN_NUM * TOTAL_PIX * 3 * sizeof(float));
	char fileName[128] = { '\0' };
	int count = 2;

	sprintf(fileName, "D:\\CoVision\\PETS2006\\input\\in000001.jpg");
	InputImg = cvLoadImage(fileName);
	cvCvtColor(InputImg, InputImg, CV_BGR2YCrCb);
	cvResize(InputImg, resizeImg);
	imgBuffer = (unsigned char*)resizeImg->imageData;

	cudaMemcpy(cudaInputBuffer, imgBuffer, TOTAL_PIX * 3, cudaMemcpyHostToDevice);
	Init << <dimGrid, dimBlock >> > (cudaInputBuffer, cudaProbability);

	float totalTime = 0;

	while (count < 1200) {

		if (InputImg != NULL) cvReleaseImage(&InputImg);
		sprintf(fileName, "D:\\CoVision\\PETS2006\\input\\in%06d.jpg", count);
		count++;

		InputImg = cvLoadImage(fileName);
		cvCvtColor(InputImg, InputImg, CV_BGR2YCrCb);
		cvResize(InputImg, resizeImg);

		cudaEvent_t start, stop;
		float time;
		cudaEventCreate(&start);
		cudaEventCreate(&stop);
		cudaEventRecord(start, 0);

		imgBuffer = (unsigned char*)resizeImg->imageData;
		cudaMemcpy(cudaInputBuffer, imgBuffer, TOTAL_PIX * 3, cudaMemcpyHostToDevice);
		cudaMemset(cudaOutputBuffer, 0, TOTAL_PIX);

		Update << <dimGrid, dimBlock >> > (cudaInputBuffer, cudaOutputBuffer, cudaProbability);
		cudaDeviceSynchronize();
		cudaMemcpy(outBuffer, cudaOutputBuffer, TOTAL_PIX, cudaMemcpyDeviceToHost);

		cudaEventRecord(stop, 0);
		cudaEventSynchronize(stop);
		cudaEventElapsedTime(&time, start, stop);
		cudaEventDestroy(start);
		cudaEventDestroy(stop);
		totalTime += time;

		//cvShowImage("Input", resizeImg);
		//cvShowImage("Foreground", OutputImg);
		//cvWaitKey(1);
	}

	cout << "fps: " << 1200 / (totalTime / 1000) << endl;
	cout << totalTime << endl;

	cudaFree(cudaInputBuffer);
	cudaFree(cudaOutputBuffer);
	cudaFree(cudaProbability);
	cudaInputBuffer = NULL;
	cudaOutputBuffer = NULL;
	cudaProbability = NULL;
	imgBuffer = NULL;

	delete[] cudaProbability;
	if (InputImg != NULL) cvReleaseImage(&InputImg);
	if (OutputImg != NULL) cvReleaseImage(&OutputImg);
	if (resizeImg != NULL) cvReleaseImage(&resizeImg);
	InputImg = NULL;
	OutputImg = NULL;
	resizeImg = NULL;
	return 0;
}