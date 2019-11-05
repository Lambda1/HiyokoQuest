#ifndef __TITLE_SCENE_HPP__
#define __TITLE_SCENE_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <ctime>

#include "../BaseScene/BaseScene.hpp"
#include "..//..//PrintString/PrintString.hpp"

#include "..//..//GameMaster/RougeLikeMap/RougeLikeMap.hpp"
#include "..//..//DrawGame/DrawGame.hpp"
#include "..//..//GameMaster/Character/Enemy/Enemy.hpp"

#include "..//Scene.hpp"

/* ��{�v���O����: (https://)github.com/Lambda1/my_library/tree/master/Game/TitleScene */

class TitleScene : public BaseScene
{
	enum class BUTTON : int
	{
		UP = static_cast<int>('w'),
		DOWN = static_cast<int>('s'),
		OK = static_cast<int>('x'),
		CANCEL = static_cast<int>('z'),
		NONE = 0
	};
	enum class MENU : int
	{
		START,
		AI,
		NETWORK,
		QUIT,

		SIZE
	};
	BUTTON key_pos;
	PrintString opengl_string;

	const float menu_locate_y = -0.3f, menu_locate_x = -0.1f; /* ���j���[�ʒu */
	const float menu_rate = 0.15f; /* ���j���[�̏㉺�� */
	const float tri_size = 0.05f; /* �O�p�`�̃T�C�Y */
	const float bias_rate = 2.5f, bias_y = 0.03f, bias_size = 0.15f; /* �J�[�\���o�C�A�X */
	const int width = 900, height = 600; /* ��ʃT�C�Y(main�Ɠ���) */

	int cursor_location = static_cast<int>(MENU::START); /* �J�[�\���ʒu */

	/* �f���p�}�b�v */
	RougeLikeMap<unsigned char> demo_map;
	inline static constexpr int demo_map_width = 50, demo_map_height = 40;
	inline static constexpr int demo_map_room_num = 10;
	/* �f���p�L�����N�^ */
	/* NOTE: �����͓G�L��������, �����ڂ͎�l�� */
	Enemy demo_enemy;
	/* �f���p�`��Ǘ� */
	DrawGame draw_manager;

	void DrawMenu();     /* ���j���[��ʕ\������ */
	void View2D();       /* ���ˉe���[�h */
	void View3D();       /* �������e���[�h */
	void StartMenu();    /* �X�^�[�g���j���[�\�� */
	void IsSceneTrans(); /* �V�[���J�ڔ��� */

	/* �f���p���� */
	void PlayDemo(); /* �f���Đ� */
	void CharacterMove(const DIRECTION& direct); /* �L�����N�^�ړ�(�f���p) */
	void CalcDirectionToPos(POS_TYPE* x, POS_TYPE* y, DIRECTION direct);

	public:
		TitleScene();
		~TitleScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(const bool* key_buffer,const bool* key_buffer_prev) override;
};

#endif
