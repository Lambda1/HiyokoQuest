#ifndef __OBJ_LOADER_HPP__
#define __OBJ_LOADER_HPP__

#include <iostream>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "../BMPLoader/BMPLoader.hpp"

/*
format:
v  x y z
vt u v
vn x y z
f  a/b/c
*/

namespace OBJ{
	enum OFFSET{
		VERTEX  = 3,
		TEXTURE = 2,
		NORMAL  = 3
	};
	enum INDEX{
		POLYGON = 3,
		FACE    = 3
	};
	enum VBO_ID{
		ID_VERTEX = 0,
		ID_NORMAL = 1,
		ID_UV = 2
	};
};

class ObjLoader
{
	std::vector<double> vertex, texture, normal;
	std::vector<int> face;

	std::vector<double> g_ver,g_tex,g_nor;
	int face_num;
	BMPLoader my_bmp;

	int *vbo_id_index;
	int texID;
	private:
	// OBJ Process
	bool LoadData(std::ifstream&);
	void LoadVertex(const std::string);
	void LoadTexture(const std::string);
	void LoadNorm(const std::string);
	void LoadFace(const std::string);
	void AllocateData();
	void ReleaseVector();
	public:
	ObjLoader();
	~ObjLoader();
	bool Init(const std::string filename);
	bool LoadTextureImage(const std::string filename);
	/* Getter */
	inline int GetFaceNum(){ return face_num; }
	inline int GetVertexSize(){ return static_cast<int>(g_ver.size()); }
	inline int GetUVSize(){ return static_cast<int>(g_tex.size()); }
	inline int GetNormalSize(){ return static_cast<int>(g_ver.size()); }
	inline double* GetVertexPointer(){ return g_ver.data(); }
	inline double* GetUVPointer(){ return g_tex.data(); }
	inline double* GetNormalPointer(){ return g_nor.data(); }
	inline unsigned char* GetTexBuf(){ return my_bmp.GetRawPointer(); }
	inline int GetTexWidth(){ return my_bmp.GetWidth(); }
	inline int GetTexHeight(){ return my_bmp.GetHeight(); }
	inline int GetVertexID(){ return vbo_id_index[OBJ::VBO_ID::ID_VERTEX]; }
	inline int GetNormalID(){ return vbo_id_index[OBJ::VBO_ID::ID_NORMAL]; }
	inline int GetUVID(){ return vbo_id_index[OBJ::VBO_ID::ID_UV]; }
	inline int GetTexID(){ return texID; }	
	/* Setter */
	void SetVBOId(const int start,const int end);
	inline void SetTexId(const int num){ texID = num; }
};

#endif
