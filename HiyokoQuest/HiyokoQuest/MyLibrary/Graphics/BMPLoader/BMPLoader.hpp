#ifndef __BMP_LOADER_HPP__
#define __BMP_LOADER_HPP__

#include <iostream>
#include <fstream>
#include <string>

constexpr int HEADER_SIZE = 14;
constexpr int INFO_H_SIZE = 40;

class BMPLoader{
	int width,height;
	unsigned char *raw_array;
	int *r,*g,*b;
	private:
		void HeaderLoad(std::fstream*);
		void InfoHdLoad(std::fstream*);
		void ImageDLoad(std::fstream*);
	public:
		BMPLoader();
		~BMPLoader();
		bool Init(const std::string filename);
		void UseRawData();

		/* Getter */
		inline int GetWidth(){ return width; }
		inline int GetHeight(){ return height; }
		inline int GetR(const int x,const int y){ return r[y*width+x]; }
		inline int GetG(const int x,const int y){ return g[y*width+x]; }
		inline int GetB(const int x,const int y){ return b[y*width+x]; }
		inline unsigned char* GetRawPointer(){ return raw_array; }
		inline int GetRaw(const int x,const int y){ return raw_array[y*width+x]; }
};

#endif
