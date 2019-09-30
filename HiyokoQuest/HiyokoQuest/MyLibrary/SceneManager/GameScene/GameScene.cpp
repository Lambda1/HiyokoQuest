#include "./GameScene.hpp"

#include <iostream>

GameScene::GameScene()
{
	scene_number = SCENE::GAME;
	std::cout << "GAME CON" << std::endl;
}

GameScene::~GameScene()
{
	std::cout << "GAME DES" << std::endl;
}

void GameScene::Update()
{
	/* ゲーム更新 */
	game_master.Update();

	/* ゲームオーバー判定 */
	if (game_master.IsGameOver()) {
		scene_number = SCENE::RESULT;
	}
}

void GameScene::Draw()
{
	glutSwapBuffers();
}

void GameScene::KeyInput(bool* key_now, bool* key_prev)
{
	/* ゲームマスタへキー入力を転送 */
	game_master.KeyInput(key_now,key_prev);
}