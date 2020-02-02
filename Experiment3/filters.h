#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>

using namespace cv;

Mat filter_mean(Mat input_image, int kernel_size, bool padding){
	Mat out = input_image.clone();

	int span = (kernel_size - 1) / 2;
	float value;
	int i_, j_;

	if (padding){
		for (int x = 0; x < out.rows; x++)
			for (int y = 0; y < out.cols; y++){
				Vec3b &output_intensity = out.at<Vec3b>(x,y);
				for (int z = 0; z < out.channels(); z++){
					value = 0;
					for (int i = x-span; i <= x+span; i++)
						for (int j = y-span; j <= y+span; j++){
							i_ = i;
							j_ = j;
							if(i_ < 0) i_ = 0;
							if(j_ < 0) j_ = 0;
							if(i_ >= out.rows) i_ = out.rows-1;
							if(j_ >= out.cols) j_ = out.cols-1;
							Vec3b input_intensity = input_image.at<Vec3b>(i_,j_);
							value += (1.0 * input_intensity.val[z]) / (kernel_size * kernel_size);
						}
					output_intensity.val[z] = (int)value;
				}
			}
	}

	return out;
}

Mat filter_median(Mat input_image, int kernel_size, bool padding){
	Mat out = input_image.clone();

	int span = (kernel_size - 1) / 2;
	int* values = new int [kernel_size*kernel_size];
	int k_count;
	int i_, j_;

	if (padding){
		for (int x = 0; x < out.rows; x++)
			for (int y = 0; y < out.cols; y++){
				Vec3b &output_intensity = out.at<Vec3b>(x,y);
				for (int z = 0; z < out.channels(); z++){
					k_count = 0;
					for (int i = x-span; i <= x+span; i++)
						for (int j = y-span; j <= y+span; j++){
							i_ = i;
							j_ = j;
							if(i_ < 0) i_ = 0;
							if(j_ < 0) j_ = 0;
							if(i_ >= out.rows) i_ = out.rows-1;
							if(j_ >= out.cols) j_ = out.cols-1;
							Vec3b input_intensity = input_image.at<Vec3b>(i_,j_);
							values[k_count++] = input_intensity.val[z];
						}
					std::sort(values, values+k_count);
					output_intensity.val[z] = values[k_count/2];
				}
			}
	}

	return out;
}

Mat filter_prewitt(Mat input_image, int kernel_size, bool padding) {
	Mat out = input_image.clone();

	int span = (kernel_size - 1) / 2;
	float value;
	int i_, j_;

	if (padding) {
		for (int x = 0; x < out.rows; x++)
			for (int y = 0; y < out.cols; y++) {
				Vec3b& output_intensity = out.at<Vec3b>(x, y);
				for (int z = 0; z < out.channels(); z++) {
					value = 0;
					for (int i = x - span; i <= x + span; i++) {
						for (int j = y - span; j <= y + span; j++) {
							i_ = i;
							j_ = j;
							if (i_ < 0) i_ = 0;
							if (j_ < 0) j_ = 0;
							if (i_ >= out.rows) i_ = out.rows - 1;
							if (j_ >= out.cols) j_ = out.cols - 1;
							int sign_ = 1;
							if (j > y)sign_ = -1;
							if (j == y)sign_ = 0;
							Vec3b input_intensity = input_image.at<Vec3b>(i_, j_);
							value += (1.0 * sign_ * input_intensity.val[z]) / (kernel_size * span);
						}
						output_intensity.val[z] = (int)value;
					}
				}
			}
	}

	return out;
}




