#ifndef __RESULT_SCENE_HPP__
#define __RESULT_SCENE_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../BaseScene/BaseScene.hpp"

#include "..//Scene.hpp"

class ResultScene : public BaseScene
{
	enum class BUTTON : int
	{
		OK = static_cast<int>('s'),
		NONE = 0
	};

	BUTTON key_pos;

	public:
		ResultScene();
		~ResultScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(const bool* key_now,const bool* key_prev) override;
};

#endif
