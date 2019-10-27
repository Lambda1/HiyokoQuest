#include "./GameScene.hpp"

#include <iostream>

GameScene::GameScene() :
	game_master(nullptr)
{
	game_master = new GameMaster;
	scene_number = SCENE::GAME;
}

GameScene::~GameScene()
{
	if (game_master) {
		delete game_master;
		game_master = nullptr;
	}
}

void GameScene::Update()
{
	/* �Q�[���X�V */
	game_master->Update();

	/* �Q�[���I�[�o�[���� */
	if (game_master->IsGameOver()) {
		scene_number = SCENE::RESULT;
	}
}

void GameScene::Draw()
{
	glutSwapBuffers();
}

void GameScene::KeyInput(const bool* key_on,const bool* key_off)
{
	/* �Q�[���}�X�^�փL�[���͂�]�� */
	game_master->KeyInput(key_on,key_off);
}