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

Mat gaussian_filter(int kernel_size)
{
	double stdv = 1.0;
	double r, s = 2.0 * stdv * stdv;  // Assigning standard deviation to 1.0
	double sum = 0.0;   // Initialization of sum for normalization
	float** kernel = (float**)malloc(kernel_size * sizeof(float*));
	for (int i = 0; i < kernel_size; i++)
		kernel[i] = (float*)malloc(kernel_size * sizeof(float))

	for (int x = -span; x <= span; x++) // Loop to generate kernel
	{
		for (int y = -span; y <= span; y++)
		{
			r = (x * x + y * y);
			kernel[x + span][y + span] = (exp(-(r) / s)) / ((22/7) *s);
			sum += kernel[x + span][y + span];
		}
	}

	for (int i = 0; i < kernel_size; i++) // Loop to normalize the kernel
		for (int j = 0; j < kernel_size; j++)
			kernel[i][j] /= sum;


	gaussian_kernel = Mat(kernel_size, kernel_size, CV_32F, kernel);

	return gaussian_kernel;

}

Mat convolute(Mat input_image,bool padding,Mat kernel,int norm_factor) {
	Mat out = input_image.clone();

	int span = (kernel.rows - 1) / 2;
	float value;
	int i_, j_;

	if (padding) {
		for (int x = 0; x < out.rows; x++)
			for (int y = 0; y < out.cols; y++) {
				Vec3b& output_intensity = out.at<Vec3b>(x, y);
				for (int z = 0; z < out.channels(); z++) {
					value = 0;
					for (int i = 0; i < kernel.rows; i++) {
						for (int j = 0; j < kernel.cols; j++) {
							i_ = x-span+i;
							j_ = y-span+j;
							if (i_ < 0) i_ = 0;
							if (j_ < 0) j_ = 0;
							if (i_ >= out.rows) i_ = out.rows - 1;
							if (j_ >= out.cols) j_ = out.cols - 1;							
							Vec3b input_intensity = input_image.at<Vec3b>(x, y);
							value += (1.0 *kernel.at<uchar>(i,j)* input_intensity.val[z])/norm_factor;
						}
						output_intensity.val[z] = (int)value;
					}
				}
			}
	}

	return out;
}

float sobel_3[3][3]={{1,2,1},{0,0,0},(-1,-2,-1));//norm_factor=4
float sobel_5[5][5] = { {1,4,6,4,1},{2,8,12,8,2},{0,0,0,0,0},{-2,-8,-12,-8,-2} ,{-1,-4,-6,-4,-1} };//norm_factor=128
float sobel_7[7][7] = { {1., 6., 15., 20., 15., 6., 1.},
                        {4., 24., 60., 80., 60., 24., 4.},
                        {5., 30., 75., 100., 75., 30., 5.},
                        {0., 0., 0., 0., 0., 0., 0.},
                        {-5., -30., -75., -100., -75., -30., -5.},
                        {-4., -24., -60., -80., -60., -24., -4.},
                        { -1., -6., -15., -20., -15., -6., -1.} };//norm_factor=2048

float sobel_9[9][9] = { {1., 8., 28., 56., 70., 56., 28., 8., 1.} ,
{6., 48., 168., 336., 420., 336., 168., 48., 6.},
{14., 112., 392., 784., 980., 784., 392., 112., 14.},
{14., 112., 392., 784., 980., 784., 392., 112., 14.},
{0., 0., 0., 0., 0., 0., 0., 0., 0.},
{-14., -112., -392., -784., -980., -784., -392., -112., -14.},
{-14., -112., -392., -784., -980., -784., -392., -112., -14.},
{-6., -48., -168., -336., -420., -336., -168., -48., -6.},
{ -1., -8., -28., -56., -70., -56., -28., -8., -1.}};//norm_factor=32768
		     

