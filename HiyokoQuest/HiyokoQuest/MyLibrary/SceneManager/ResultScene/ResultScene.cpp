#include "./ResultScene.hpp"

#include <iostream>

ResultScene::ResultScene() :
	key_pos(BUTTON::NONE)
{
	scene_number = SCENE::RESULT;
}

ResultScene::~ResultScene()
{
}

void ResultScene::View2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0); /* -1.0Å`1.0ÇÃãÛä‘Ç…ê›íË */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void ResultScene::View3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, static_cast<double>(width / height), 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void ResultScene::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	View2D();
	print_manager.DrawStrings("YOU ARE DEAD", -0.15f, 0.0f, 0.0f, PS::COLOR::RED);
	View3D();

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