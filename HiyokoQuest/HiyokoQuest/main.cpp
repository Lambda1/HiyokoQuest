#include <string>

#include ".//MyLibrary/MyGL/MyGL.hpp"
#include ".//MyLibrary/SceneManager/SceneManager.hpp"
#include "./MyLibrary/FPS_Controller/FPS_Controller.hpp"

#define aDEBUG

int main(int argc, char* argv[])
{
	const std::string window_name = "Sample"; /* ウィンドウタイトル */
	const int width = 900, height = 600; /* 画面の大きさ */
	
	const int frame_rate_key = 1; /* F1: fps切替キー */
	const double main_fps[] = {1000.0/60.0, 1000.0/140.0, 1000.0/1000.0}; /* fps処理 */
	const int index_size = sizeof(main_fps) / sizeof(double);
	int fps_index = 0; /* fps選択インデックス */

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
		
#ifdef DEBUG
		fps_manager.FPS_Checker(); /* DEBUG: FPSチェック */
#endif

		/* FPS切替 */
		if (opengl_manager.GetSpKeyUp() == frame_rate_key) { fps_index = (fps_index + 1) % index_size; }
		
		/* 60 || 120fps処理 */
		if (fps_manager.Timer_Nfps(main_fps[fps_index]))
		{
			scene_manager.KeyInput(opengl_manager.GetKeyBuffer()); /* キーボード入力 */
			
			scene_manager.Update();    /* シーン更新 */
			scene_manager.Draw();      /* シーン描画 */

			scene_manager.TransScene(); /* シーン遷移処理 */
		}
	}
	return 0;
}