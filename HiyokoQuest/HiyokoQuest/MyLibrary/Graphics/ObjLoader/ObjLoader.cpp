#include "./ObjLoader.hpp"

/* private */
bool ObjLoader::LoadData(std::ifstream &obj_file)
{
	std::string f_line;
	while(std::getline(obj_file,f_line)){
		if(f_line.find(std::string("v ")) != std::string::npos) LoadVertex(f_line);
		if(f_line.find(std::string("vt ")) != std::string::npos) LoadTexture(f_line);
		if(f_line.find(std::string("vn ")) != std::string::npos) LoadNorm(f_line);
		if(f_line.find(std::string("f ")) != std::string::npos) LoadFace(f_line);
	}
	AllocateData();

	return true;
}
void ObjLoader::LoadVertex(const std::string v_line)
{
	std::stringstream stream(v_line.substr(2)); /* v + space = 2-char */
	std::string buf;
	while(std::getline(stream,buf,' ')) vertex.push_back(std::stod(buf));
}
void ObjLoader::LoadTexture(const std::string t_line)
{
	std::stringstream stream(t_line.substr(3)); /* vt + space = 3-char */
	std::string buf;
	while(std::getline(stream,buf,' ')) texture.push_back(std::stod(buf));
}
void ObjLoader::LoadNorm(const std::string n_line)
{
	std::stringstream stream(n_line.substr(3)); /* vn + space = 3-char */
	std::string buf;
	while(std::getline(stream,buf,' ')) normal.push_back(std::stod(buf));
}
void ObjLoader::LoadFace(const std::string f_line)
{
	face_num++;
	std::stringstream stream(f_line.substr(2)); /* f + space = 2-char */
	std::string buf;
	while(std::getline(stream,buf,' ')){
		std::string slash_buf;
		std::stringstream ss(buf);
		while(std::getline(ss,slash_buf,'/')) face.push_back(std::stoi(slash_buf));
	}
}
void ObjLoader::AllocateData()
{
	int loop_cnt = static_cast<int>(face.size()/OBJ::INDEX::FACE);
	for(int i = 0;i < loop_cnt;i++){
		for (int j = 0; j < OBJ::OFFSET::VERTEX; j++)
		{
			int sub_index = i * (OBJ::INDEX::POLYGON); /* C26451 */
			int index = OBJ::OFFSET::VERTEX * (face[sub_index]-1) + j; /* C26451 */
			g_ver.push_back(vertex[index]);
		}
		for (int j = 0; j < OBJ::OFFSET::TEXTURE; j++)
		{
			int sub_index = i * (OBJ::INDEX::POLYGON) + 1; /* C26451 */
			int index = OBJ::OFFSET::TEXTURE * (face[sub_index] - 1) + j; /* C26451 */
			g_tex.push_back(texture[index]);
		}
		for (int j = 0; j < OBJ::OFFSET::NORMAL; j++)
		{
			int sub_index = i * (OBJ::INDEX::POLYGON) + 2; /* C26451 */
			int index = OBJ::OFFSET::NORMAL * (face[sub_index] - 1) + j; /* C26451 */
			g_nor.push_back(normal[index]);
		}
	}
	ReleaseVector();
}
void ObjLoader::ReleaseVector()
{
	std::vector<double>().swap(vertex);
	std::vector<double>().swap(texture);
	std::vector<double>().swap(normal);
	std::vector<int>().swap(face);
}
/* public */
ObjLoader::ObjLoader()
{
	vbo_id_index = nullptr;
	face_num = 0;
	texID = 0;
}

ObjLoader::~ObjLoader()
{
	if(vbo_id_index) delete[] vbo_id_index;
	std::vector<double>().swap(g_ver);
	std::vector<double>().swap(g_tex);
	std::vector<double>().swap(g_nor);
}
bool ObjLoader::Init(const std::string filename)
{
	std::ifstream input;

	input.open(filename,std::ios::in);
	if(input.fail()){
		std::cout << "Cannot open " << filename << std::endl;
		return false;
	}
	if(!LoadData(input)){
		std::cout << "ERROR: LOAD FILE" << std::endl;
		return false;
	}
	input.close();

	return true;
}
bool ObjLoader::LoadTextureImage(const std::string filename)
{
	if(!my_bmp.Init(filename)){ return false; }
	my_bmp.UseRawData();
	return true;
}
/* Setter */
void ObjLoader::SetVBOId(const int start,const int end)
{
	int index = (end - start); /* C26451‘Îô */
	vbo_id_index = new int[index];
	for(int i = start;i < end;i++) vbo_id_index[(i-start)] = i;
}
