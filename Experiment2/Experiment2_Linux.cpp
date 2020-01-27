#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int* get_cdf(Mat input, int *cdf){
	int pdf[256];
	for (int i = 0; i < 256; i++)
		pdf[i] = 0;

	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++){
			Vec3b intensity = input.at<Vec3b>(i, j);
			for (int k = 0; k < input.channels(); k++)
				pdf[intensity.val[k]]++;
		}

	cdf[0] = pdf[0];
	// cout << cdf[0] << endl;
	for (int i = 1; i < 256; i++)
		cdf[i] = cdf[i-1] + pdf[i];
}

Mat equalize(Mat input, int *cdf){
	Mat out = input.clone();

	int maxVal = cdf[255];
	int newVal;

	for (int i = 0; i < input.rows; i++)
		for (int j = 0; j < input.cols; j++){
			Vec3b input_intensity = input.at<Vec3b>(i, j);
			Vec3b &output_intensity = out.at<Vec3b>(i, j);
			for (int k = 0; k < input.channels(); k++){
				newVal = (int)(256*(cdf[input_intensity.val[k]]/(1.0 * maxVal)));
				output_intensity.val[k] = newVal;
			}
		}

	return out;
}

int main(int argc, char const *argv[])
{
	Mat input_image;
	input_image = imread("Exp2/lena_gray_dark.jpg", CV_LOAD_IMAGE_UNCHANGED);
	int* cdf;
	cdf = (int*)malloc(256*sizeof(int));
	get_cdf(input_image, cdf);
	Mat equalized_image = equalize(input_image, cdf);
	imwrite("lena_gray_dark_eq.jpg", equalized_image);
	// for (int i = 0; i < 256; i++)
	// 	cout << cdf[i] << endl;
	return 0;
}