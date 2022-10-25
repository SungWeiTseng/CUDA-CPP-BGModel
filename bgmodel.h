#pragma once
#define ALPHA 0.001f
#define BETA  1.0f - ALPHA
#define THRESHOLD 0.3f
#define BIN_NUM 32
#define DELTA_B 256 / BIN_NUM

#define PIX_WID 640
#define PIX_HEI 480
#define TOTAL_PIX PIX_WID*PIX_HEI

#define THREADS_PER_BLOCK 16	//16*16
#define BLOCKS_WID PIX_WID / THREADS_PER_BLOCK
#define BLOCKS_HEI PIX_HEI / THREADS_PER_BLOCK