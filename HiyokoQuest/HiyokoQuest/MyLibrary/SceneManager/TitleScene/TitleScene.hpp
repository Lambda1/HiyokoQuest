#ifndef __TITLE_SCENE_HPP__
#define __TITLE_SCENE_HPP__

#include "../BaseScene/BaseScene.hpp"

/* äÓñ{ÉvÉçÉOÉâÉÄ: (https://)github.com/Lambda1/my_library/tree/master/Game/TitleScene */

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
	BUTTON key_pos;
	public:
		TitleScene();
		~TitleScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(bool* key_now, bool* key_prev) override;
};

#endif
