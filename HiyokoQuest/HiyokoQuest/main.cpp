#include <iostream>
#include <string>

#include ".//MyLibrary/MyGL/MyGL.hpp"
#include ".//MyLibrary/SceneManager/SceneManager.hpp"

int main(int argc, char* argv[])
{
	const std::string window_name = "Sample"; /* ウィンドウタイトル */
	const int width = 900, height = 600; /* 画面の大きさ */

	MyGL opengl_manager;
	SceneManager scene_manager;

	/* OpenGLの初期化処理 */
	opengl_manager.Init(window_name, width, height, argc, argv);
	opengl_manager.EntryCallBack(false);

	/* シーン管理の初期化処理 */
	scene_manager.ChangeScene(SCENE::TITLE);

	while (true)
	{
		opengl_manager.MainLoop(); /* OpenGLのループ処理 */

		scene_manager.KeyInput(opengl_manager.GetKey(),opengl_manager.GetKeyUp()); /* キーボード入力 */
		scene_manager.Update();    /* シーン更新 */
		scene_manager.Draw();      /* シーン描画 */

		opengl_manager.ResetKey(); /* キーリセット */
	}

	return 0;
}