#include <iostream>
#include <string>

#include ".//MyLibrary/MyGL/MyGL.hpp"
#include ".//MyLibrary/SceneManager/SceneManager.hpp"

int main(int argc, char* argv[])
{
	const std::string window_name = "Sample"; /* �E�B���h�E�^�C�g�� */
	const int width = 900, height = 600; /* ��ʂ̑傫�� */

	MyGL opengl_manager;
	SceneManager scene_manager;

	/* OpenGL�̏��������� */
	opengl_manager.Init(window_name, width, height, argc, argv);
	opengl_manager.EntryCallBack(false);

	/* �V�[���Ǘ��̏��������� */
	scene_manager.ChangeScene(SCENE::TITLE);

	while (true)
	{
		opengl_manager.MainLoop(); /* OpenGL�̃��[�v���� */

		scene_manager.KeyInput(opengl_manager.GetKey(),opengl_manager.GetKeyUp()); /* �L�[�{�[�h���� */
		scene_manager.Update();    /* �V�[���X�V */
		scene_manager.Draw();      /* �V�[���`�� */

		opengl_manager.ResetKey(); /* �L�[���Z�b�g */
	}

	return 0;
}