#include "./BMPLoader.hpp"

BMPLoader::BMPLoader() :
	width(0), height(0),
	raw_array(nullptr),
	r(nullptr), g(nullptr), b(nullptr)
{}

BMPLoader::~BMPLoader()
{
	if(raw_array) delete[] raw_array;
	if(r) delete[] r;
	if(g) delete[] g;
	if(b) delete[] b;
}

bool BMPLoader::Init(const std::string filename)
{
	std::fstream bmp(filename,std::ios::in|std::ios::binary);
	if(!bmp.is_open()){ std::cout << "ERROR: Open " << filename << std::endl; return false; }

	HeaderLoad(&bmp);
	InfoHdLoad(&bmp);
	ImageDLoad(&bmp);

	bmp.close();

	return true;
}

void BMPLoader::HeaderLoad(std::fstream *bmp)
{
	/*
	 2B file type
	 4B file size
	 2B+2B reserved area
	 4B OFFSET
	*/
	char data[HEADER_SIZE];
	bmp->read(data,sizeof(char)*HEADER_SIZE);
}
void BMPLoader::InfoHdLoad(std::fstream *bmp)
{
	/*
	 4B header size
	 4B width
	 4B height
	 3B plane
	 2B pixel
	 4B pressure size
	 4B size
	 4B ppm
	 4B palet ppm
	 4B index
	*/
	char data[INFO_H_SIZE];
	bmp->read(data,sizeof(char)*INFO_H_SIZE);
	memcpy(&width,data+static_cast<char>(4), sizeof(char)*static_cast<char>(4));
	memcpy(&height,data+static_cast<char>(8),sizeof(char)*static_cast<char>(4));
}
void BMPLoader::ImageDLoad(std::fstream *bmp)
{
	int index = width * height; /* C26451 */
	r = new int[index];
	g = new int[index];
	b = new int[index];
	int length = width*3 + width%4;
	unsigned char *data = new unsigned char[length];

	for(int i = 0;i < height;i++){
		bmp->read(reinterpret_cast<char*>(data),1*length);
		for(int j = 0;j < width;j++){
			b[(height-i-1)*width+j] = static_cast<int>(data[j*3+0]);
			g[(height-i-1)*width+j] = static_cast<int>(data[j*3+1]);
			r[(height-i-1)*width+j] = static_cast<int>(data[j*3+2]);
		}
	}
	delete[] data;
}
		
void BMPLoader::UseRawData()
{
	int index = (width * 3 + width % 4) * height; /* C26451 */
	raw_array = new unsigned char[index];
	for(int i = 0;i < height;i++){
		for(int j = 0;j < width;j++){
			raw_array[(height-i-1)*(width*3)+j*3 + 0] = static_cast<unsigned char>(r[i*width+j]);
			raw_array[(height-i-1)*(width*3)+j*3 + 1] = static_cast<unsigned char>(g[i*width+j]);
			raw_array[(height-i-1)*(width*3)+j*3 + 2] = static_cast<unsigned char>(b[i*width+j]);
		}
	}
}
