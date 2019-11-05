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

/* 基本プログラム: (https://)github.com/Lambda1/my_library/tree/master/Game/TitleScene */

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

	const float menu_locate_y = -0.3f, menu_locate_x = -0.1f; /* メニュー位置 */
	const float menu_rate = 0.15f; /* メニューの上下幅 */
	const float tri_size = 0.05f; /* 三角形のサイズ */
	const float bias_rate = 2.5f, bias_y = 0.03f, bias_size = 0.15f; /* カーソルバイアス */
	const int width = 900, height = 600; /* 画面サイズ(mainと同じ) */

	int cursor_location = static_cast<int>(MENU::START); /* カーソル位置 */

	/* デモ用マップ */
	RougeLikeMap<unsigned char> demo_map;
	inline static constexpr int demo_map_width = 50, demo_map_height = 40;
	inline static constexpr int demo_map_room_num = 10;
	/* デモ用キャラクタ */
	/* NOTE: 処理は敵キャラだが, 見た目は主人公 */
	Enemy demo_enemy;
	/* デモ用描画管理 */
	DrawGame draw_manager;

	void DrawMenu();     /* メニュー画面表示処理 */
	void View2D();       /* 正射影モード */
	void View3D();       /* 透視投影モード */
	void StartMenu();    /* スタートメニュー表示 */
	void IsSceneTrans(); /* シーン遷移判定 */

	/* デモ用処理 */
	void PlayDemo(); /* デモ再生 */
	void CharacterMove(const DIRECTION& direct); /* キャラクタ移動(デモ用) */
	void CalcDirectionToPos(POS_TYPE* x, POS_TYPE* y, DIRECTION direct);

	public:
		TitleScene();
		~TitleScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(const bool* key_buffer,const bool* key_buffer_prev) override;
};

#endif
