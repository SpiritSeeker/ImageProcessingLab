#include <iostream>
#include <fstream>

using namespace std;

struct BMPHeader{
	uint16_t file_type;
	uint32_t file_size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset_data;
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression_type;
	uint32_t image_size;
	uint32_t x_res;
	uint32_t y_res;
	uint32_t colors_used;
	uint32_t colors_imp;
};

int main(int argc, char** argv)
{
	
	return 0;
}
