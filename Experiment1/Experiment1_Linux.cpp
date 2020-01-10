#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

// Structure to hold the header information
struct BMPHeader{
	unsigned char file_type[2];
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
		const char* bm_filename;

	public:
		struct BMPHeader bm_head;
		unsigned char*** bm_pixelValues;

		BitMap(){

		}

		void read(const char* filename){
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
			bm_head.planes = *(unsigned short*)(bm_header+26);
			bm_head.bit_count = *(unsigned short*)(bm_header+28);
			bm_head.compression_type = *(unsigned int*)(bm_header+30);
			bm_head.image_size = *(unsigned int*)(bm_header+34);
			bm_head.x_res = *(unsigned int*)(bm_header+38);
			bm_head.y_res = *(unsigned int*)(bm_header+42);
			bm_head.colors_used = *(unsigned int*)(bm_header+46);
			bm_head.colors_imp = *(unsigned int*)(bm_header+50);

			// cout << bm_head.file_type << endl;
			// cout << bm_head.file_size << endl;
			// cout << bm_head.reserved1 << endl;
			// cout << bm_head.reserved2 << endl;
			// cout << bm_head.offset_data << endl;
			// cout << bm_head.size << endl;
			// cout << bm_head.width << endl;
			// cout << bm_head.height << endl;
			// cout << bm_head.planes << endl;
			// cout << bm_head.bit_count << endl;
			// cout << bm_head.compression_type << endl;
			// cout << bm_head.image_size << endl;
			// cout << bm_head.x_res << endl;
			// cout << bm_head.y_res << endl;
			// cout << bm_head.colors_used << endl;
			// cout << bm_head.colors_imp << endl;

			auto dataSize = ((bm_head.width * 3 + 3) & (~3)) * bm_head.height;
			vector<char> img(dataSize);

			// cout << dataSize << endl;

			bm_stream.read(img.data(), img.size());

			// for (int i = 0; i < dataSize; i+=3)
			// {
			// 	cout << (unsigned int)(img[i] & 0xff) << ", ";
			// 	cout << (unsigned int)(img[i+1] & 0xff) << ", ";
			// 	cout << (unsigned int)(img[i+2] & 0xff) << endl;
			// }

			if(bm_head.bit_count == 24){
				bm_pixelValues = (unsigned char***)malloc(bm_head.height*sizeof(unsigned char**));
				for (int i = 0; i < bm_head.height; i++){
					bm_pixelValues[i] = (unsigned char**)malloc(bm_head.width*sizeof(unsigned char*));
					for (int j = 0; j < bm_head.width; j++)
						bm_pixelValues[i][j] = (unsigned char*)malloc(3*sizeof(unsigned char));
				}

				for (int i = 0; i < bm_head.height; i++){
					for (int j = 0; j < bm_head.width; j++){
						a = img[(bm_head.width*3*i)+j*3] & 0xff;
						cout << int(a) << ", ";
						bm_pixelValues[bm_head.height-1-i][j][2] = a;
						a = img[(bm_head.width*3*i)+j*3+1] & 0xff;
						cout << int(a) << ", ";
						bm_pixelValues[bm_head.height-1-i][j][1] = a;
						a = img[(bm_head.width*3*i)+j*3+2] & 0xff;
						cout << int(a) << endl;
						bm_pixelValues[bm_head.height-1-i][j][0] = a;	
					}
				}
			}

			bm_stream.close();
		}

		int save(const char* filename){
			ofstream bm_stream(filename, ios::binary);

			// cout << sizeof(BMPHeader) << endl;
			// bm_stream.write((char *)(&bm_head), sizeof(BMPHeader));

			// bm_head.planes *= 256;
			// bm_head.bit_count *= 256;

			bm_stream << bm_head.file_type[0];
			bm_stream << bm_head.file_type[1];
			bm_stream.write((char*)(&bm_head.file_size), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.reserved1), sizeof(unsigned short));
			bm_stream.write((char*)(&bm_head.reserved2), sizeof(unsigned short));
			bm_stream.write((char*)(&bm_head.offset_data), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.size), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.width), sizeof(int));
			bm_stream.write((char*)(&bm_head.height), sizeof(int));
			bm_stream.write((char*)(&bm_head.planes), sizeof(unsigned short));
			bm_stream.write((char*)(&bm_head.bit_count), sizeof(unsigned short));
			bm_stream.write((char*)(&bm_head.compression_type), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.image_size), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.x_res), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.y_res), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.colors_used), sizeof(unsigned int));
			bm_stream.write((char*)(&bm_head.colors_imp), sizeof(unsigned int));

			// bm_head.planes /= 256;
			// bm_head.bit_count /= 256;
			
			unsigned char a;
			for (int i = 0; i < bm_head.height; i++){
				for (int j = 0; j < bm_head.width; j++){
					bm_stream << bm_pixelValues[bm_head.height-1-i][j][2];
					// bm_stream << hex << a;
					bm_stream << bm_pixelValues[bm_head.height-1-i][j][1];
					// bm_stream << hex << a;
					bm_stream << bm_pixelValues[bm_head.height-1-i][j][0];	
					// bm_stream << hex << a;
				}
			}
			bm_stream.close();

			return 1;
		}
		// ~BitMap();
		
};

// BitMap flip_bitmap(BitMap inp){
// 	BitMap out;
// 	out.bm_head = inp.bm_head;
// 	int temp_h = out.bm_head.height;
// 	out.bm_head.height = out.bm_head.width;
// 	out.bm_head.width = temp_h;

// 	for (int i = 0; i < out.bm_head.height; i++)
// 		for (int j = 0; j < out.bm_head.width; j++)
// 			out.bm_pixelValues[i][j] = inp.bm_pixelValues[out.bm_head.width - j][out.bm_head.height - i];

// 	return out;
// }

int main(int argc, char** argv)
{
	BitMap lena;
	lena.read("lena_colored_256.bmp");
	// for (int i = 0; i < 128; i++)
	// 	for (int j = 0; j < 128; j++){
	// 		lena.bm_pixelValues[i][j][2] = (unsigned char)255;
	// 		lena.bm_pixelValues[i][j][1] = (unsigned char)0;
	// 		lena.bm_pixelValues[i][j][0] = (unsigned char)0;
	// 	}
	// cout << int(lena.bm_pixelValues[0][0][0]) << endl;
	// cout << int(lena.bm_pixelValues[0][0][1]) << endl;
	// cout << int(lena.bm_pixelValues[0][0][2]) << endl;
	lena.save("lena_ex.bmp");
	return 0;
}
