#include "./GameScene.hpp"

#include <iostream>

GameScene::GameScene()
{
	scene_number = SCENE::GAME;
}

GameScene::~GameScene()
{
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

void GameScene::KeyInput(const bool* key_on,const bool* key_off)
{
	/* ゲームマスタへキー入力を転送 */
	game_master.KeyInput(key_on,key_off);
}