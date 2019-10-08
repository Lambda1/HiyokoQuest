#ifndef __DRAW_GAME_HPP__
#define __DRAW_GAME_HPP__

#include "..//VBOLoader/VBOLoader.hpp"
#include "..//MyShader/MyShader.hpp"

#include "..//Graphics/ResourcePath.hpp"

#include "..//Graphics/ObjLoader/ObjLoader.hpp"

#include "..//GameMaster/RougeLikeMap/MapSet.hpp"
#include "../GameMaster/Character/Character.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

class DrawGame
{
	/* .obj��� */
	const int obj_info     = 3; /* ���_�E�@���EUV���W */
	const int model_number = 1; /* �g�p����3D���f���� */
	/* �`��N���b�s���O�͈� */
	const int range_x = 7, range_y = 9;


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
	void DrawObj(ObjLoader *obj_data, const float &x, const float &z, const float &ang); /* VBO+Shader��.obj��`�� */
public:
		DrawGame();
		~DrawGame();

		void Init(); /* ���������� */

		/* �J�������� */
		template<class T>
		void CameraPos(const T& pos_x, const T& pos_y, const T& pos_z, const T& gaze_x, const T& gaze_y, const T& gaze_z)
		{
			glLoadIdentity();
			gluLookAt(pos_x, pos_y, pos_z, gaze_x, gaze_y, gaze_z, 0.0, 1.0, 0.0);
		}
		/* �`�揈�� */
		/* ��ʃN���A */
		inline void DrawInit()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}
		/* ���C���w�ɂ��}�b�v�\�� */
		void DrawMap(const unsigned char *dungeon,const int &width,const int &height); /* �}�b�v�w�\�� */
		/* ���W�ɂ�����}�b�v�\�� (�N���b�s���O�Ɏg�p) */
		void DrawMap(const unsigned char* dungeon, const int& width, const int& height, const int& px, const int& py);
		/* ���C���w�ɂ��L�����N�^�\�� */
		void DrawCharacter(const unsigned char* dungeon, const int& width, const int& height); /* �L�����N�^�[�\�� */
		/* ���W�ɂ�����L�����N�^�\�� (�N���b�s���O�Ɏg�p) */
		void DrawCharacter(Character* ch_obj);
};

#endif
