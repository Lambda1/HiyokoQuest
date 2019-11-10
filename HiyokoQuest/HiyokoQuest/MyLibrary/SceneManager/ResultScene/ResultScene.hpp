#ifndef __RESULT_SCENE_HPP__
#define __RESULT_SCENE_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../BaseScene/BaseScene.hpp"

#include "..//..//PrintString/PrintString.hpp"

#include "..//Scene.hpp"

class ResultScene : public BaseScene
{
	enum class BUTTON : int
	{
		OK = static_cast<int>('s'),
		NONE = 0
	};

	const int width = 900, height = 600; /* 画面サイズ(mainと同じ) */

	BUTTON key_pos;

	PrintString print_manager;

	void View2D();       /* 正射影モード */
	void View3D();       /* 透視投影モード */

	public:
		ResultScene();
		~ResultScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(const bool* key_now,const bool* key_prev) override;
};

#endif
