#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat filter_morph(Mat input_image, int kernel_size, bool padding, bool erode){
	Mat out = input_image.clone();

	int span = (kernel_size - 1) / 2;
	int* values = new int [kernel_size*kernel_size];
	int k_count;
	int i_, j_;

	if (padding){
		for (int x = 0; x < out.rows; x++)
			for (int y = 0; y < out.cols; y++){
				uchar &output_intensity = out.at<uchar>(x,y);
				k_count = 0;
				for (int i = x-span; i <= x+span; i++)
					for (int j = y-span; j <= y+span; j++){
						i_ = i;
						j_ = j;
						if(i_ < 0) i_ = 0;
						if(j_ < 0) j_ = 0;
						if(i_ >= out.rows) i_ = out.rows-1;
						if(j_ >= out.cols) j_ = out.cols-1;
						uchar input_intensity = input_image.at<uchar>(i_,j_);
						values[k_count++] = input_intensity;
					}
				std::sort(values, values+k_count);
				if (erode)
					output_intensity = values[0];
				else
					output_intensity = values[k_count-1];
			}
	}

	return out;
}

Mat filter_morph_open(Mat input_image, int kernel_size, bool open){
	if (open){
		Mat temp = filter_morph(input_image, kernel_size, true, true);
		Mat out = filter_morph(temp, kernel_size, true, false);
		return out;
	}
	else{
		Mat temp1 = filter_morph(input_image, kernel_size, true, false);
		cout << "Open Temp" << endl;
		Mat out1 = filter_morph(temp1, kernel_size, true, true);
		return out1;
	}
}

int main(){
	Mat input_image = imread("expt5/ricegrains.bmp", CV_LOAD_IMAGE_UNCHANGED);
	if (!input_image.data) cout << "Err" << endl;
	Mat erode_img = filter_morph_open(input_image, 3, true);
	imwrite("open.jpg", erode_img);
	erode_img = filter_morph_open(input_image, 3, false);
	imwrite("close.jpg", erode_img);
}