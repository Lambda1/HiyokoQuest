#include "./TitleScene.hpp"

#include <iostream>

TitleScene::TitleScene() :
	key_pos(BUTTON::NONE),
	demo_map(static_cast<int>(std::time(nullptr))),
	demo_enemy(1.0f, MAPSET::DATA::PLAYER),
	goal_x(0), goal_y(0)
{
	scene_number = SCENE::TITLE;

	/* デモマップ初期化 */
	demo_map.Init(demo_map_width, demo_map_height, demo_map_room_num);
	demo_map.SetBaseInfo(static_cast<MAP_TYPE>(MAPSET::DATA::NONE), static_cast<MAP_TYPE>(MAPSET::DATA::ROOM), static_cast<MAP_TYPE>(MAPSET::DATA::ROAD), static_cast<MAP_TYPE>(MAPSET::DATA::WALL));
	demo_map.Generate();
	demo_map.GetRoomPos(&goal_x, &goal_y);
	/* デモキャラ初期化 */
	int enemy_pos_x, enemy_pos_y;
	demo_map.GetRoomPos(&enemy_pos_x, &enemy_pos_y);
	demo_enemy.InitPos(static_cast<POS_TYPE>(enemy_pos_x), static_cast<POS_TYPE>(enemy_pos_y));
	demo_enemy.SetAI(ENEMY_AI::MODE::A_STAR);
	demo_enemy.SetTargetPos(goal_x,goal_y);

	draw_manager.Init();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	DrawMenu();
}

void TitleScene::Draw()
{
	glutSwapBuffers();
}

void TitleScene::KeyInput(const bool* key_buffer,const bool* key_buffer_prev)
{
	key_pos = BUTTON::NONE;
	for(int i = 0;i < alphabet;i++)
		if (key_buffer[i] && !key_buffer_prev[i])
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
void TitleScene::View3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, static_cast<double>(width / height), 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
void TitleScene::StartMenu()
{
	/* ウィンドウ表示 */
	glBegin(GL_QUADS);
	{
		glColor4f(0.8f, 0.8f, 0.8f, 0.8f);
		glVertex3f(menu_locate_x * -2.0f, menu_locate_y * 0.5f, 0.1f);
		glVertex3f(menu_locate_x *  2.5f, menu_locate_y * 0.5f, 0.1f);
		glVertex3f(menu_locate_x *  2.5f, menu_locate_y * 2.8f, 0.1f);
		glVertex3f(menu_locate_x * -2.0f, menu_locate_y * 2.8f, 0.1f);
	}
	glEnd();

	/* 文字メニュー表示 */
	opengl_string.DrawStrings("START",   menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::START),   0, PS::COLOR::BLACK);
	opengl_string.DrawStrings("AI-MODE", menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::AI),      0, PS::COLOR::SILVER);
	opengl_string.DrawStrings("NETWORK", menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::NETWORK), 0, PS::COLOR::SILVER);
	opengl_string.DrawStrings("QUIT",    menu_locate_x, menu_locate_y - menu_rate * static_cast<int>(MENU::QUIT),    0, PS::COLOR::BLACK);

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

void TitleScene::IsSceneTrans()
{
	if (key_pos == BUTTON::OK)
	{
		switch (static_cast<MENU>(cursor_location))
		{
		case MENU::START:
			scene_number = SCENE::GAME; break;
		case MENU::AI:
			break;
		case MENU::NETWORK:
			break;
		case MENU::QUIT:
			game_quite = true; break;
		default:
			break;
		}
	}
}

void TitleScene::DrawMenu()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* デモ再生 */
	View3D();
	PlayDemo();

	/* 2Dメニュー表示 */
	View2D();
	StartMenu();

	/* 3D表示 */
	View3D();

	/* シーン遷移処理 */
	IsSceneTrans();
}
/* デモ再生 */
void TitleScene::PlayDemo()
{
	draw_manager.CameraPos<GLfloat>(demo_enemy.GetPosPX(), 10.0f, demo_enemy.GetPosPY() + 10.0f, demo_enemy.GetPosPX(), 0.0f, demo_enemy.GetPosPY());
	draw_manager.DrawMap(demo_map.GetDungeon(), demo_map_width, demo_map_height, static_cast<int>(demo_enemy.GetPosX()), static_cast<int>(demo_enemy.GetPosY()));
	if (demo_enemy.GetPosX() == demo_enemy.GetPosPX() && demo_enemy.GetPosY() == demo_enemy.GetPosPY())
	{
		DIRECTION direct = demo_enemy.AI_Mode(demo_map.GetDungeon(), demo_map_width, demo_map_height);
		CharacterMove(direct);
	}
	else { demo_enemy.MoveAnimation(); }
	draw_manager.DrawCharacter(&demo_enemy,demo_map_width,demo_map_height, static_cast<int>(demo_enemy.GetPosX()), static_cast<int>(demo_enemy.GetPosY()));

	if (demo_enemy.GetDirect() == DIRECTION::NONE)
	{
		demo_map.GetRoomPos(&goal_x, &goal_y);
		demo_enemy.SetTargetPos(goal_x, goal_y);
	}
}
/* デモキャラ移動 */
/* NOTE: GameMasterクラスより, 処理効率を向上させた(デモ特化). */
void TitleScene::CharacterMove(const DIRECTION &direct)
{
	POS_TYPE pos_x = demo_enemy.GetPosX(), pos_y = demo_enemy.GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, direct);
	MAPSET::DATA pos_data = static_cast<MAPSET::DATA>(demo_map.GetDungeon()[static_cast<int>(pos_y) * demo_map_width + static_cast<int>(pos_x)]);
	if (pos_data == MAPSET::DATA::ROOM || pos_data == MAPSET::DATA::ROAD) { demo_enemy.Move(direct); }
}
/* HACK: 同じ処理があるため, 共通化すべき. */
void TitleScene::CalcDirectionToPos(POS_TYPE* x, POS_TYPE* y, DIRECTION direct)
{
	switch (direct)
	{
	case DIRECTION::EAST:
		(*x)++; break;
	case DIRECTION::WEST:
		(*x)--; break;
	case DIRECTION::SOUTH:
		(*y)++; break;
	case DIRECTION::NORTH:
		(*y)--; break;
	case DIRECTION::SOUTH_EAST:
		(*x)++; (*y)++; break;
	case DIRECTION::SOUTH_WEST:
		(*x)--; (*y)++; break;
	case DIRECTION::NORTH_EAST:
		(*x)++; (*y)--; break;
	case DIRECTION::NORTH_WEST:
		(*x)--; (*y)--; break;
	default:
		break;
	}
}