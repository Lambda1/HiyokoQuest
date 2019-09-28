#include "./TitleScene.hpp"

#include <iostream>

TitleScene::TitleScene() :
	key_pos(BUTTON::NONE)
{
	std::cout << "TITLE CON" << std::endl;
}

TitleScene::~TitleScene()
{
	std::cout << "TITLE DES" << std::endl;
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
	std::cout << static_cast<int>(key_pos) << std::endl;
}

void TitleScene::KeyInput(bool* key_now, bool* key_prev)
{
	key_pos = BUTTON::NONE;
	for(int i = 0;i < alphabet;i++)
		if (key_now[i] && !key_prev[i])
		{
			switch (static_cast<BUTTON>(key_bias+i))
			{
			case BUTTON::UP:     key_pos = BUTTON::UP;     break;
			case BUTTON::DOWN:   key_pos = BUTTON::DOWN;   break;
			case BUTTON::OK:     key_pos = BUTTON::OK;     break;
			case BUTTON::CANCEL: key_pos = BUTTON::CANCEL; break;
			default:             key_pos = BUTTON::NONE;   break;
			}
		}
}