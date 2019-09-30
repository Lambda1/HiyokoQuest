#ifndef __TITLE_SCENE_HPP__
#define __TITLE_SCENE_HPP__

#include <GL/freeglut.h>

#include "../BaseScene/BaseScene.hpp"
#include "..//..//PrintString/PrintString.hpp"

#include "..//Scene.hpp"

/* ��{�v���O����: (https://)github.com/Lambda1/my_library/tree/master/Game/TitleScene */

class TitleScene : public BaseScene
{
	enum class BUTTON : int
	{
		UP     = static_cast<int>('w'),
		DOWN   = static_cast<int>('x'),
		OK     = static_cast<int>('s'),
		CANCEL = static_cast<int>('z'),
		NONE   = 0
	};
	enum class MENU : int
	{
		START,
		PLAYER2,
		NETWORK,
		QUIT,

		SIZE
	};
	BUTTON key_pos;
	PrintString opengl_string;
	
	const float menu_locate_y = -0.3f, menu_locate_x = -0.1f; /* ���j���[�ʒu */
	const float menu_rate   = 0.15f; /* ���j���[�̏㉺�� */
	const float tri_size = 0.05f; /* �O�p�`�̃T�C�Y */
	const float bias_rate = 2.5f, bias_y = 0.03f, bias_size = 0.15f; /* �J�[�\���o�C�A�X */
	const int width = 900, height = 600; /* ��ʃT�C�Y(main�Ɠ���) */

	int cursor_location = static_cast<int>(MENU::START); /* �J�[�\���ʒu */

	void DrawMenu();     /* ���j���[��ʕ\������ */
	void View2D();       /* ���ˉe���[�h */
	void View3D();       /* �������e���[�h */
	void StartMenu();    /* �X�^�[�g���j���[�\�� */
	void IsSceneTrans(); /* �V�[���J�ڔ��� */

	public:
		TitleScene();
		~TitleScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(bool* key_now, bool* key_prev) override;
};

#endif
