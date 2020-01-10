#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

// Structure to hold the header information
typedef struct {
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
} BMPHeader;

typedef struct {
	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char reserved;
} color_table;

class BitMap
{
	private:
		const char* bm_filename;

	public:
		BMPHeader bm_head;
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


			if(bm_head.bit_count == 24){
				auto dataSize = ((bm_head.width * 3 + 3) & (~3)) * bm_head.height;
				vector<char> img(dataSize);

				bm_stream.read(img.data(), img.size());

				bm_pixelValues = (unsigned char***)malloc(bm_head.height*sizeof(unsigned char**));
				for (int i = 0; i < bm_head.height; i++){
					bm_pixelValues[i] = (unsigned char**)malloc(bm_head.width*sizeof(unsigned char*));
					for (int j = 0; j < bm_head.width; j++)
						bm_pixelValues[i][j] = (unsigned char*)malloc(3*sizeof(unsigned char));
				}

				for (int i = 0; i < bm_head.height; i++){
					for (int j = 0; j < bm_head.width; j++){
						a = img[(bm_head.width*3*i)+j*3] & 0xff;
						bm_pixelValues[bm_head.height-1-i][j][2] = a;
						a = img[(bm_head.width*3*i)+j*3+1] & 0xff;
						bm_pixelValues[bm_head.height-1-i][j][1] = a;
						a = img[(bm_head.width*3*i)+j*3+2] & 0xff;
						bm_pixelValues[bm_head.height-1-i][j][0] = a;	
					}
				}
			}

			if(bm_head.bit_count == 8){
				auto dataSize = bm_head.width * bm_head.height;
				vector<char> img(dataSize);

				bm_stream.read(img.data(), img.size());

				bm_pixelValues = (unsigned char***)malloc(bm_head.height*sizeof(unsigned char**));
				for (int i = 0; i < bm_head.height; i++){
					bm_pixelValues[i] = (unsigned char**)malloc(bm_head.width*sizeof(unsigned char*));
					for (int j = 0; j < bm_head.width; j++)
						bm_pixelValues[i][j] = (unsigned char*)malloc(1*sizeof(unsigned char));
				}

				for (int i = 0; i < bm_head.height; i++){
					for (int j = 0; j < bm_head.width; j++){
						a = img[(bm_head.width*i)+j] & 0xff;
						bm_pixelValues[bm_head.height-1-i][j][2] = a;
					}
				}
			}

			bm_stream.close();
		}

		int save(const char* filename){
			ofstream bm_stream(filename, ios::binary);

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

			if (bm_head.bit_count == 24){
				for (int i = 0; i < bm_head.height; i++){
					for (int j = 0; j < bm_head.width; j++){
						bm_stream << bm_pixelValues[bm_head.height-1-i][j][2];
						bm_stream << bm_pixelValues[bm_head.height-1-i][j][1];
						bm_stream << bm_pixelValues[bm_head.height-1-i][j][0];	
					}
				}
			}

			if (bm_head.bit_count == 8){
				color_table clr_tbl[256];

				for (int i = 0; i < 256; i++){
					clr_tbl[i].B = i;
					clr_tbl[i].G = i;
					clr_tbl[i].R = i;
				}

				bm_stream.write((char*)(&clr_tbl), 256*sizeof(color_table));

				for (int i = 0; i < bm_head.height; i++)
					for (int j = 0; j < bm_head.width; j++){
						bm_stream << bm_pixelValues[bm_head.height-1-i][j][0];
					}
			}

			bm_stream.close();

			return 1;
		}
		// ~BitMap();
		
};

BitMap flip_bitmap(BitMap inp){
	BitMap out;
	out.bm_head = inp.bm_head;
	int temp_h = out.bm_head.height;
	out.bm_head.height = out.bm_head.width;
	out.bm_head.width = temp_h;

	out.bm_pixelValues = (unsigned char***)malloc(out.bm_head.height*sizeof(unsigned char**));
	for (int i = 0; i < out.bm_head.height; i++){
		out.bm_pixelValues[i] = (unsigned char**)malloc(out.bm_head.width*sizeof(unsigned char*));
		for (int j = 0; j < out.bm_head.width; j++)
			out.bm_pixelValues[i][j] = (unsigned char*)malloc(3*sizeof(unsigned char));
	}

	for (int i = 0; i < out.bm_head.height; i++)
		for (int j = 0; j < out.bm_head.width; j++)
			for (int k = 0; k < 3; k++)
				out.bm_pixelValues[i][j][k] = inp.bm_pixelValues[out.bm_head.width - 1 - j][out.bm_head.height - 1 - i][k];

	return out;
}

BitMap bgr_to_gray(BitMap inp, const char* mode){
	BitMap out;
	out.bm_head = inp.bm_head;
	out.bm_head.bit_count = 8;
	out.bm_head.file_size = (out.bm_head.height * out.bm_head.width) + sizeof(BMPHeader) + 256*sizeof(color_table);
	out.bm_head.offset_data += 256 * sizeof(color_table);
	out.bm_head.image_size = out.bm_head.height * out.bm_head.width;

	out.bm_pixelValues = (unsigned char***)malloc(out.bm_head.height*sizeof(unsigned char**));
	for (int i = 0; i < out.bm_head.height; i++){
		out.bm_pixelValues[i] = (unsigned char**)malloc(out.bm_head.width*sizeof(unsigned char*));
		for (int j = 0; j < out.bm_head.width; j++)
			out.bm_pixelValues[i][j] = (unsigned char*)malloc(1*sizeof(unsigned char));
	}

	float value;

	for (int i = 0; i < out.bm_head.height; i++)
		for (int j = 0; j < out.bm_head.width; j++){
			if (mode == "avg")
				value = (int(inp.bm_pixelValues[i][j][0]) + int(inp.bm_pixelValues[i][j][1]) + int(inp.bm_pixelValues[i][j][2])) / 3.0;
			
			if (mode == "min"){
				value = 255;
				for (int k = 0; k < 3; k++)
					if (int(inp.bm_pixelValues[i][j][k]) < value)
						value = int(inp.bm_pixelValues[i][j][k]);
			}
			
			if (mode == "max"){
				value = 0;
				for (int k = 0; k < 3; k++)
					if (int(inp.bm_pixelValues[i][j][k]) > value)
						value = int(inp.bm_pixelValues[i][j][k]);
			}

			out.bm_pixelValues[i][j][0] = int(value);
		}

	return out;
}

int main(int argc, char** argv)
{
	BitMap lena;
	lena.read("lena_colored_256.bmp");
	flip_bitmap(lena).save("lena_flip.bmp");
	bgr_to_gray(lena, "avg").save("lena_gray_avg.bmp");
	bgr_to_gray(lena, "min").save("lena_gray_min.bmp");
	bgr_to_gray(lena, "max").save("lena_gray_max.bmp");
	return 0;
}
