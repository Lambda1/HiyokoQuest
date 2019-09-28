#ifndef __RESULT_SCENE_HPP__
#define __RESULT_SCENE_HPP__

#include "../BaseScene/BaseScene.hpp"

class ResultScene : public BaseScene
{
	public:
		ResultScene();
		~ResultScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(bool* key_now, bool* key_prev) override;
};

#endif
