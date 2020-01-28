#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "filters.h"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	Mat im = imread("med.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Mat out = filter_median(im, 3, true);
	imwrite("median.jpg", out);
	return 0;
}