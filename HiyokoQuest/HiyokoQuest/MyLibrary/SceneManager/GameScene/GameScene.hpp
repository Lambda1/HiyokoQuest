#ifndef __GAME_SCENE_HPP__
#define __GAME_SCENE_HPP__

#include "../BaseScene/BaseScene.hpp"

#include "..//Scene.hpp"

#include "..//..//GameMaster/GameMaster.hpp"

/* ��{�v���O����: (https://)github.com/Lambda1/my_library/tree/master/Game/GameScene */

class GameScene : public BaseScene
{
	GameMaster *game_master; /* �Q�[�������N���X */

	public:
		GameScene();
		~GameScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(const bool* key_now,const bool* key_prev) override;
};

#endif
