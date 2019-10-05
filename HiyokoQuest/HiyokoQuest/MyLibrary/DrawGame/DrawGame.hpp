#ifndef __DRAW_GAME_HPP__
#define __DRAW_GAME_HPP__

#include "..//VBOLoader/VBOLoader.hpp"
#include "..//MyShader/MyShader.hpp"

#include "..//Graphics/ResourcePath.hpp"

#include "..//Graphics/ObjLoader/ObjLoader.hpp"

#include "..//GameMaster/RougeLikeMap/MapSet.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

class DrawGame
{
	const int obj_info     = 3; /* ���_�E�@���EUV���W */
	const int model_number = 1; /* �g�p����3D���f���� */

	/* 3D���f�� �֌W */
	ObjLoader* player;
	const int player_id_start = 0;

	/* Shader�֌W */
	MyShader shader_manager;
	inline static const std::string obj_shader_var[] = {"pos", "uv", "texture"}; /* ����͖@�����͎g��Ȃ� */

	/* VBO�֌W */
	VBOLoader vbo_manager;

private:
	/* �������֌W */
	/* VBO������ */
	void SetVBOInfo(ObjLoader *obj_data, const int id_start); 	/* VBO��.obj�t�@�C����ǉ� */

	/* �`��֌W */
	void DrawObj(ObjLoader *obj_data, const float &x, const float &z); /* VBO+Shader��.obj��`�� */
public:
		DrawGame();
		~DrawGame();

		void Init(); /* ���������� */

		void CameraPos(const int pos_x, const int pos_y, const int pos_z, const int gaze_x,const int gaze_y,const int gaze_z); /* �J�����ʒu */

		void DrawMap(const unsigned char *dungeon,const int &width,const int &height); /* �}�b�v�w�\�� */
		void DrawCharacter(const unsigned char* dungeon, const int& width, const int& height); /* �L�����N�^�[�\�� */
};

#endif
