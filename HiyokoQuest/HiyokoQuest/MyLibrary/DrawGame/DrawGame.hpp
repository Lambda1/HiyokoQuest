#ifndef __DRAW_GAME_HPP__
#define __DRAW_GAME_HPP__

#include "..//VBOLoader/VBOLoader.hpp"
#include "..//MyShader/MyShader.hpp"

#include "..//Graphics/ResourcePath.hpp"

#include "..//Graphics/ObjLoader/ObjLoader.hpp"

#include "..//GameMaster/RougeLikeMap/MapSet.hpp"
#include "../GameMaster/Character/Character.hpp"

#include "../PrintString/PrintString.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

class DrawGame
{
	enum class ENEMY_INFO : int
	{
		ENEMY,
	};

	/* .obj��� */
	const int obj_info     = 3; /* ���_�E�@���EUV���W */
	const int model_number = 5; /* �g�p����3D���f���� */
	/* �`��N���b�s���O�͈� */
	const int range_x = 7, range_y = 9;

	/* ��ʃT�C�Y */
	int width, height;
	
	/* �X�e�[�^�X�o�[�ʒu */
	const float up_x = -0.9f, up_y = 0.80f;
	const float frame_size_w = 0.6f, frame_size_h = 0.12f;
	const float wide_length = 0.15f;
	/* �X�e�[�^�X�������Έʒu */
	const float st_up_x = up_x + 0.01f, st_up_y = up_y + 0.03f;

	/* 3D���f�� �֌W */
	/* ��l�� */
	ObjLoader* player;
	const int player_id_start = 0;
	/* �� */
	ObjLoader* wall;
	const int wall_id_start  = player_id_start + obj_info;
	/* �K�i */
	ObjLoader* stair;
	const int stair_id_start = wall_id_start + obj_info;
	/* �� */
	ObjLoader* tyle;
	const int tyle_id_start  = stair_id_start + obj_info;
	/* �G */
	std::vector<ObjLoader*> enemy;
	std::vector<int> enemy_id_start;

	/* Shader�֌W */
	MyShader shader_manager;
	inline static const std::string obj_shader_var[] = {"pos", "uv", "texture"}; /* ����͖@�����͎g��Ȃ� */

	/* VBO�֌W */
	VBOLoader vbo_manager;

	/* �����\���֘A */
	PrintString print_manager;

	/* �t���[�������֘A */
	int  frame_counter;    /* �t���[���J�E���^ */
	bool is_frame_counter; /* �t���[���������� */

private:
	/* �������֌W */
	/* .obj���[�h */
	ObjLoader* LoadObjFile(const std::string &obj_path);
	/* VBO������ */
	void SetVBOInfo(ObjLoader *obj_data, const int id_start); 	/* VBO��.obj�t�@�C����ǉ� */

	/* �`��֌W */
	void DrawMode2D(); /* ���s���e */
	void DrawMode3D(); /* �������e */
	/* �X�e�[�^�X�\�� */
	inline void DrawRect(const float& x, const float& y)
	{
		glBegin(GL_QUADS);
		{
			glColor3f(0.4f, 0.4f, 0.4f);
			glVertex3f(x, y, 0.1f);
			glVertex3f(x + frame_size_w, y, 0.1f);
			glVertex3f(x + frame_size_w, y + frame_size_h, 0.1f);
			glVertex3f(x, y + frame_size_h, 0.1f);
		}
		glEnd();
	}
	inline std::string GetStringLV(const int& level)
	{
		return (std::string("Lv: ") + std::to_string(level));
	}
	inline std::string GetStringHP(const int& hp, const int& max_hp)
	{
		return (std::string("HP: ") + std::to_string(hp) + std::string(" / ") + std::to_string(max_hp));
	}
	inline std::string GetStringFL(const int& floor)
	{
		return (std::to_string(floor) + std::string(" F"));
	}
	/* .obj�\�� */
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
		/* ���W�ɂ�����L�����N�^�\�� */
		void DrawCharacter(Character* ch_data);
		/* ���W�ɂ�����L�����N�^�\�� (�N���b�v���O�Ɏg�p) */
		void DrawCharacter(Character* ch_data, const int& width, const int& height,const int& px,const int& py);

		/* �X�e�[�^�X�\�� */
		void DrawStatusBar(Character* ch_data, const int& floor);

		/* �K�w�\�� */
		bool DrawBlackScreen(const int &floor_num, const int &frame_time);

		/* �Z�b�^ */
		inline void SetSize(const int& width, const int& height) { this->width = width, this->height = height; }
};

#endif