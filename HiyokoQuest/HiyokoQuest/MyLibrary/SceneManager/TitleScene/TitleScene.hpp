#ifndef __TITLE_SCENE_HPP__
#define __TITLE_SCENE_HPP__

#include "../BaseScene/BaseScene.hpp"
#include "..//..//PrintString/PrintString.hpp"

/* 基本プログラム: (https://)github.com/Lambda1/my_library/tree/master/Game/TitleScene */

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
	
	const float menu_locate_y = -0.3f, menu_locate_x = -0.1f; /* メニュー位置 */
	const float menu_rate   = 0.15f; /* メニューの上下幅 */
	const float tri_size = 0.05f; /* 三角形のサイズ */
	const float bias_rate = 2.5f, bias_y = 0.03f, bias_size = 0.15f; /* カーソルバイアス */

	int cursor_location = static_cast<int>(MENU::START); /* カーソル位置 */

	void DrawMenu();
	void View2D();    /* 正射影モード */
	void StartMenu(); /* スタートメニュー表示 */

	public:
		TitleScene();
		~TitleScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(bool* key_now, bool* key_prev) override;
};

#endif
