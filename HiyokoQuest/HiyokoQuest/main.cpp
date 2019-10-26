#include <string>

#include ".//MyLibrary/MyGL/MyGL.hpp"
#include ".//MyLibrary/SceneManager/SceneManager.hpp"
#include "./MyLibrary/FPS_Controller/FPS_Controller.hpp"

int main(int argc, char* argv[])
{
	const std::string window_name = "Sample"; /* �E�B���h�E�^�C�g�� */
	const int width = 900, height = 600; /* ��ʂ̑傫�� */
	
	const int frame_rate_key = 1; /* F1: fps�ؑփL�[ */
	const double main_fps[2] = {1000.0/60.0, 1000.0/120.0}; /* fps���� */
	int fps_index = 0; /* fps�I���C���f�b�N�X */

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
		
		//fps_manager.FPS_Checker(); /* DEBUG: FPS�`�F�b�N */

		/* FPS�ؑ� */
		if (opengl_manager.GetSpKeyUp() == frame_rate_key) { fps_index = (fps_index + 1) % 2; }
		
		/* 60 || 120fps���� */
		if (fps_manager.Timer_Nfps(main_fps[fps_index]))
		{
			scene_manager.KeyInput(opengl_manager.GetKeyBuffer()); /* �L�[�{�[�h���� */
			
			scene_manager.Update();    /* �V�[���X�V */
			scene_manager.Draw();      /* �V�[���`�� */

			scene_manager.TransScene(); /* �V�[���J�ڏ��� */
		}
	}

	return 0;
}