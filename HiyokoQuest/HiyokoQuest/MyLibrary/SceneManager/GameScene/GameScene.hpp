#ifndef __GAME_SCENE_HPP__
#define __GAME_SCENE_HPP__

#include "../BaseScene/BaseScene.hpp"

/* Šî–{ƒvƒƒOƒ‰ƒ€: (https://)github.com/Lambda1/my_library/tree/master/Game/GameScene */

class GameScene : public BaseScene
{
	public:
		GameScene();
		~GameScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(bool* key_now, bool* key_prev) override;
};

#endif
