#include "./ResultScene.hpp"

#include <iostream>

ResultScene::ResultScene() :
	key_pos(BUTTON::NONE)
{
	scene_number = SCENE::RESULT;
	std::cout << "RESULT CON" << std::endl;
}

ResultScene::~ResultScene()
{
	std::cout << "RESULT DES" << std::endl;
}

void ResultScene::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	if (key_pos == BUTTON::OK) scene_number = SCENE::TITLE;
}

void ResultScene::Draw()
{
	glutSwapBuffers();
}

void ResultScene::KeyInput(const bool* key_now,const bool* key_prev)
{
	key_pos = BUTTON::NONE;
	for (int i = 0; i < alphabet; i++)
		if (key_now[i] && !key_prev[i])
		{
			switch (static_cast<BUTTON>(key_bias + i))
			{
			case BUTTON::OK:     key_pos = BUTTON::OK;     break;
			default:             key_pos = BUTTON::NONE;   break;
			}
		}
}