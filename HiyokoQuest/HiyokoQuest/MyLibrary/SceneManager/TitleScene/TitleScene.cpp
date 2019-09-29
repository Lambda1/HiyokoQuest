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
	DrawMenu();
}

void TitleScene::Draw()
{
	glutSwapBuffers();
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

/* private */
void TitleScene::View2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0); /* -1.0～1.0の空間に設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void TitleScene::StartMenu()
{
	/* 文字メニュー表示 */
	opengl_string.DrawStrings("START",    menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::START),   0, PS::COLOR::BLACK);
	opengl_string.DrawStrings("2-PLAYER", menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::PLAYER2), 0, PS::COLOR::SILVER);
	opengl_string.DrawStrings("NETWORK",  menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::NETWORK), 0, PS::COLOR::SILVER);
	opengl_string.DrawStrings("QUIT",     menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::QUIT),    0, PS::COLOR::BLACK);

	/* キー入力によるカーソル位置算出 */
	if (key_pos == BUTTON::DOWN) { cursor_location = (cursor_location + 1) % static_cast<int>(MENU::SIZE); }
	else if (key_pos == BUTTON::UP) { cursor_location = (cursor_location + static_cast<int>(MENU::SIZE) - 1) % static_cast<int>(MENU::SIZE); }

	/* カーソル表示 */
	glPushMatrix();
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(menu_locate_x*bias_rate, (menu_locate_y+bias_y)-cursor_location*bias_size, 0);
		glBegin(GL_TRIANGLES);
		{
			glVertex2f(tri_size, tri_size);
			glVertex2f(tri_size, -tri_size);
			glVertex2f(tri_size * 2, (tri_size - tri_size) / 2);
		}
		glEnd();
	}
	glPopMatrix();
}

void TitleScene::DrawMenu()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	View2D();
	StartMenu();
}