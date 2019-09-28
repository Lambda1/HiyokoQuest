#include "./GameScene.hpp"

#include <iostream>

GameScene::GameScene()
{
	std::cout << "GAME CON" << std::endl;
}

GameScene::~GameScene()
{
	std::cout << "GAME DES" << std::endl;
}

void GameScene::Update()
{
}

void GameScene::Draw()
{
}

void GameScene::KeyInput(bool* key_now, bool* key_prev)
{

}