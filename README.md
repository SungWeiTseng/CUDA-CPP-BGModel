# CUDA-CPP-BGModel

## 介紹
- 基於N bins直方圖統計Block內顏色資訊對背景建模，以顏色機率判斷前景或背景。
- 使用CUDA C++ 加速的直方圖背景模型[1] Baseline，以一個block一個thread做處理。


## Dataset
下載時序影像 e.g. http://changedetection.net/

**reference**
[1] Lin, Horng-Horng, Li-Chen Shih, and Jen-Hui Chuang. "Efficient background modeling using nonparametric histogramming." 2013 Seventh International Conference on Distributed Smart Cameras (ICDSC). IEEE, 2013.
