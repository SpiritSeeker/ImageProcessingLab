# README for Experiment1_Linux.cpp

## Functionality
* Reads a .bmp image
* Print the header information
* Flip the image and save it
* Convert the image to grayscale using average, minimum and maximum and save the images

## Usage
The code is written keeping in mind C++11 standard, therefore it has to be compiled with the c++11 flag. The input file path and output file paths are to be passed as command line arguments.
``` shell
g++ Experiment1_Linux.cpp -std=c++11 /path/to/input/image.bmp /path/to/flipped/image.bmp
/path/to/gray/avg/image.bmp /path/to/gray/min/image.bmp /path/to/gray/max/image.bmp
```