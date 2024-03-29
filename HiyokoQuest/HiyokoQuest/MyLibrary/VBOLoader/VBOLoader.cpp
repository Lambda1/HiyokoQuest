#include "../VBOLoader/VBOLoader.hpp"

/* コンストラクタ, デストラクタ */
VBOLoader::VBOLoader() :
	VboId(nullptr),
	vbo_index(0)
{}

VBOLoader::~VBOLoader()
{
	glDeleteBuffers(vbo_index, VboId);
	for (std::vector<GLuint>::iterator itr = texID.begin(); itr != texID.end(); ++itr)
	{
		glDeleteBuffers(1, &(*itr));
		glDeleteTextures(1, &(*itr));
	}
	if(VboId){ delete[] VboId; }
	std::vector<GLuint>().swap(texID);
}

/* 初期化関連 */
void VBOLoader::Init(int num)
{
	VboId = new GLuint[num];
	vbo_index = static_cast<GLuint>(num);
	glGenBuffers(vbo_index, VboId);
}
GLuint VBOLoader::InitTex()
{
	GLuint tmpID;
	glGenBuffers(1, &tmpID);
	texID.push_back(tmpID);
	return tmpID;
}
void VBOLoader::SetBuffer(int size,double* ptr,int index)
{
	glBindBuffer(GL_ARRAY_BUFFER, VboId[index]);
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(double), ptr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
	
void VBOLoader::SetTexBuffer(GLuint id,int width,int height,unsigned char* ptr)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/* Process */
void VBOLoader::BindBuffer(int index)
{
	glBindBuffer(GL_ARRAY_BUFFER, VboId[index]);
}
void VBOLoader::BindBufferUseShader(int index,int value,int dimension)
{
	glBindBuffer(GL_ARRAY_BUFFER, VboId[index]);
	glVertexAttribPointer(value, dimension, GL_DOUBLE, GL_FALSE, 0, 0);
}
void VBOLoader::BindTexture(GLuint id)
{
	glBindTexture(GL_TEXTURE_2D, id);
}

/* VBO適用 */
void VBOLoader::EnableState(GLenum mode)
{
	glEnableClientState(mode);
}
void VBOLoader::DisableState(GLenum mode)
{
	glDisableClientState(mode);
}
void VBOLoader::EnableTex()
{
	glEnable(GL_TEXTURE_2D);
}

/* VBO解除 */
void VBOLoader::UnBind(bool tex_flag)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if(tex_flag) glBindTexture(GL_TEXTURE_2D, 0);
}

/* 描画 */
void VBOLoader::DrawArrays(GLenum mode,int size)
{
	glDrawArrays(mode, 0, size);
}
