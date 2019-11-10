#include "./TitleScene.hpp"

#include <iostream>

TitleScene::TitleScene() :
	key_pos(BUTTON::NONE),
	demo_map(static_cast<int>(std::time(nullptr))),
	demo_enemy(1.0f, MAPSET::DATA::PLAYER)
{
	scene_number = SCENE::TITLE;

	/* デモマップ初期化 */
	demo_map.Init(demo_map_width, demo_map_height, demo_map_room_num);
	demo_map.SetBaseInfo(static_cast<MAP_TYPE>(MAPSET::DATA::NONE), static_cast<MAP_TYPE>(MAPSET::DATA::ROOM), static_cast<MAP_TYPE>(MAPSET::DATA::ROAD), static_cast<MAP_TYPE>(MAPSET::DATA::WALL));
	demo_map.Generate();
	demo_map.Update();
	demo_map.GetRoomPos(&goal.x, &goal.y);
	/* デモキャラ初期化 */
	int enemy_pos_x, enemy_pos_y;
	demo_map.GetRoomPos(&enemy_pos_x, &enemy_pos_y);
	demo_map.SetField(goal.y,goal.x,static_cast<MAP_TYPE>(MAPSET::DATA::PLAYER));
	demo_enemy.InitPos(static_cast<POS_TYPE>(enemy_pos_x), static_cast<POS_TYPE>(enemy_pos_y));
	demo_enemy.SetAI(ENEMY_AI::MODE::A_STAR);
	demo_enemy.SetTargetID(MAPSET::DATA::PLAYER);

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
	DrawMenuWindow();

	/* 文字メニュー表示 */
	DrawMenuString();

	/* キー入力によるカーソル位置算出 */
	if (key_pos == BUTTON::DOWN) { cursor_location = (cursor_location + 1) % static_cast<int>(MENU::SIZE); }
	else if (key_pos == BUTTON::UP) { cursor_location = (cursor_location + static_cast<int>(MENU::SIZE) - 1) % static_cast<int>(MENU::SIZE); }

	/* カーソル表示 */
	DrawCursor();
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
	draw_manager.DrawInit();

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
	draw_manager.CameraPos<GLfloat>(demo_enemy.GetPosPX(), 15.0f, demo_enemy.GetPosPY() + 10.0f, demo_enemy.GetPosPX(), 0.0f, demo_enemy.GetPosPY());
	draw_manager.DrawMap(demo_map.GetDungeon(), demo_map_width, demo_map_height, static_cast<int>(demo_enemy.GetPosX()), static_cast<int>(demo_enemy.GetPosY()));
	if (demo_enemy.GetPosX() == demo_enemy.GetPosPX() && demo_enemy.GetPosY() == demo_enemy.GetPosPY())
	{
		DIRECTION direct = demo_enemy.AI_Mode(demo_map.GetDungeon(), demo_map_width, demo_map_height);
		CharacterMove(direct);
	}
	else { demo_enemy.MoveAnimation(); }
	if (demo_enemy.GetTurnMode() == TURN_MODE::ATTACK)
	{
		demo_map.SetField(goal.y,goal.x,static_cast<MAP_TYPE>(MAPSET::DATA::ROOM));
		demo_map.GetRoomPos(&goal.x, &goal.y);
		demo_map.SetField(goal.y, goal.x, static_cast<MAP_TYPE>(MAPSET::DATA::PLAYER));
		demo_enemy.SetSearchRouteFlag(true);
	}
	draw_manager.DrawCharacter(&demo_enemy, demo_map_width, demo_map_height, static_cast<int>(demo_enemy.GetPosX()), static_cast<int>(demo_enemy.GetPosY()));
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