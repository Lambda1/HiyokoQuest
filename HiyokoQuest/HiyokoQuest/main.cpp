#include <iostream>
#include <string>

#include ".//MyLibrary/MyGL/MyGL.hpp"
#include ".//MyLibrary/SceneManager/SceneManager.hpp"
#include "./MyLibrary/FPS_Controller/FPS_Controller.hpp"

int main(int argc, char* argv[])
{
	const std::string window_name = "Sample"; /* �E�B���h�E�^�C�g�� */
	const int width = 900, height = 600; /* ��ʂ̑傫�� */

	MyGL opengl_manager;        /* OpenGL���� */
	SceneManager scene_manager; /* �V�[������ */
	FPS_CNTR::FPS fps_manager;  /* FPS���� */

	/* OpenGL�̏��������� */
	opengl_manager.Init(window_name, width, height, argc, argv);
	opengl_manager.EntryCallBack(false);

	/* �V�[���Ǘ��̏��������� */
	scene_manager.ChangeScene(SCENE::TITLE);

	while (!scene_manager.GameQuite())
	{
		opengl_manager.MainLoop(); /* OpenGL�̃��[�v���� */
		
		fps_manager.FPS_Checker(); /* DEBUG: FPS�`�F�b�N */

		/* 60FPS���� */
		if (fps_manager.Timer_60fps())
		{
			scene_manager.KeyInput(opengl_manager.GetKeyBuffer()); /* �L�[�{�[�h���� */
			
			scene_manager.Update();    /* �V�[���X�V */
			scene_manager.Draw();      /* �V�[���`�� */

			scene_manager.TransScene(); /* �V�[���J�ڏ��� */
		}
	}

	return 0;
}