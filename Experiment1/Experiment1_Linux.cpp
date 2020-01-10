#include <iostream>
#include <fstream>
#include <math.h>

// Structure to hold the header information
struct BMPHeader{
	unsigned char file_type[3];
	unsigned int file_size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset_data;
	unsigned int size;
	int width;
	int height;
	unsigned short planes;
	unsigned short bit_count;
	unsigned int compression_type;
	unsigned int image_size;
	unsigned int x_res;
	unsigned int y_res;
	unsigned int colors_used;
	unsigned int colors_imp;
};

class BitMap
{
	private:
		struct BMPHeader bm_head;
		const char* bm_filename;

	public:
		BitMap(const char* filename){
			using namespace std;

			bm_filename = filename;
			ifstream bm_stream(filename);

			unsigned char bm_header[54];
			unsigned char a;
			for (int i = 0; i < 54; i++){
				bm_stream >> hex >> a;
				bm_header[i] = a;
			}
			
			bm_head.file_type[0] = bm_header[0];
			bm_head.file_type[1] = bm_header[1];
			bm_head.file_type[2] = 0;
			
			bm_head.file_size = *(unsigned int*)(bm_header+2);
			bm_head.reserved1 = *(unsigned short*)(bm_header+6);
			bm_head.reserved2 = *(unsigned short*)(bm_header+8);
			bm_head.offset_data = *(unsigned int*)(bm_header+10);

			bm_head.size = *(unsigned int*)(bm_header+14);
			bm_head.width = *(int*)(bm_header+18);
			bm_head.height = *(int*)(bm_header+22);
			bm_head.planes = int(bm_header[26]);
			bm_head.bit_count = int(bm_header[28]);
			bm_head.compression_type = *(unsigned int*)(bm_header+30);
			bm_head.image_size = *(unsigned int*)(bm_header+34);
			bm_head.x_res = *(unsigned int*)(bm_header+38);
			bm_head.y_res = *(unsigned int*)(bm_header+42);
			bm_head.colors_used = *(unsigned int*)(bm_header+46);
			bm_head.colors_imp = *(unsigned int*)(bm_header+50);

			cout << bm_head.file_type << endl;
			cout << bm_head.file_size << endl;
			cout << bm_head.reserved1 << endl;
			cout << bm_head.reserved2 << endl;
			cout << bm_head.offset_data << endl;
			cout << bm_head.size << endl;
			cout << bm_head.width << endl;
			cout << bm_head.height << endl;
			cout << bm_head.planes << endl;
			cout << bm_head.bit_count << endl;
			cout << bm_head.compression_type << endl;
			cout << bm_head.image_size << endl;
			cout << bm_head.x_res << endl;
			cout << bm_head.y_res << endl;
			cout << bm_head.colors_used << endl;
			cout << bm_head.colors_imp << endl;

			if(bm_head.bit_count == 24){
				
			}
		}
		// ~BitMap();
		
};

int main(int argc, char** argv)
{
	BitMap lena("lena_colored_256.bmp");
	return 0;
}
