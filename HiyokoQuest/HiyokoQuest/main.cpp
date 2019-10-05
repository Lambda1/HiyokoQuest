#include <iostream>
#include <string>

#include ".//MyLibrary/MyGL/MyGL.hpp"
#include ".//MyLibrary/SceneManager/SceneManager.hpp"
#include "./MyLibrary/FPS_Controller/FPS_Controller.hpp"

int main(int argc, char* argv[])
{
	const std::string window_name = "Sample"; /* ウィンドウタイトル */
	const int width = 900, height = 600; /* 画面の大きさ */

	MyGL opengl_manager;        /* OpenGL処理 */
	SceneManager scene_manager; /* シーン処理 */
	FPS_CNTR::FPS fps_manager;  /* FPS処理 */

	/* OpenGLの初期化処理 */
	opengl_manager.Init(window_name, width, height, argc, argv);
	opengl_manager.EntryCallBack(false);

	/* シーン管理の初期化処理 */
	scene_manager.ChangeScene(SCENE::TITLE);

	while (!scene_manager.GameQuite())
	{
		opengl_manager.MainLoop(); /* OpenGLのループ処理 */
		
		fps_manager.FPS_Checker(); /* DEBUG: FPSチェック */

		/* 60FPS処理 */
		if (fps_manager.Timer_60fps())
		{
			scene_manager.KeyInput(opengl_manager.GetKeyBuffer()); /* キーボード入力 */
			
			scene_manager.Update();    /* シーン更新 */
			scene_manager.Draw();      /* シーン描画 */

			scene_manager.TransScene(); /* シーン遷移処理 */
		}
	}

	return 0;
}