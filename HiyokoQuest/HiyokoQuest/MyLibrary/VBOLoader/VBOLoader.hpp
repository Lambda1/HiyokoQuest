#ifndef __VBO_LOADER_HPP__
#define __VBO_LOADER_HPP__

#include <GL/glew.h>

class VBOLoader
{
	GLuint *VboId;
	GLsizei vbo_index;
	public:
	VBOLoader();
	~VBOLoader();
	
	/* 初期化関連 */
	void Init(int vbo_num); /* プログラム中に確保する全バッファ数*/
	GLuint InitTex(); /* テクスチャ1枚の初期化. */
	void SetBuffer(int size,double* ptr,int index);
	void SetTexBuffer(GLuint id, int width,int height,unsigned char* ptr);
	
	/* 処理関連 */
	void BindBuffer(int index); /* バインド(value) */
	void BindBufferUseShader(int index,int value,int dimension); /* バインド(シェーダ利用) */
	void BindTexture(GLuint); /* バインド(texture) */
	
	/* VBO適用 */
	void EnableState(GLenum mode); /* 有効: mode */
	void EnableTex(); /* 有効: texture */
	
	/* VBO解除 */
	void DisableState(GLenum mode); /* 解除: mode */
	void DisableTex(); /* 解除: texture */
	void UnBind(bool tex_flag); /* 解除: Bind (true: texture解除) */
	
	/* 描画 */
	void DrawArrays(GLenum mode,int size); /* 基本は使わない */
};

#endif
