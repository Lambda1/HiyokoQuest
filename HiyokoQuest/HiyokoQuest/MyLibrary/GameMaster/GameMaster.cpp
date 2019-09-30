#include "./GameMaster.hpp"

GameMaster::GameMaster() :
	game_step(GAME_STEP::INIT),
	game_over_flag(false),
	random_seed(static_cast<int>(std::time(nullptr))), cnt_seed(0),
	game_map(nullptr) , floor_number(0), turn_number(0), room_number(0)
{
}

GameMaster::~GameMaster()
{
	if (game_map) { delete game_map; }
}

void GameMaster::Update()
{
	switch (game_step)
	{
	case GAME_STEP::INIT:
		Init(); break;
	case GAME_STEP::CREATE_MAP:
		CreateMap(); break;
	case GAME_STEP::TURN_START:
		TurnStart(); break;
	case GAME_STEP::PLAYER_TURN:
		PlayerTurn(); break;
	case GAME_STEP::ITEM_TURN:
		break;
	case GAME_STEP::ENEMY_TURN:
		break;
	case GAME_STEP::STATUS_TURN:
		break;
	case GAME_STEP::TURN_END:
		break;
	default:
		break;
	}

	if (game_step >= GAME_STEP::TURN_START) {
		DrawMap();
		DrawStatus();
	}
}

void GameMaster::KeyInput(bool* key_on, bool* key_off)
{
}

/* private */
/* ゲーム処理 */
void GameMaster::Init()
{
	std::cout << "GAME INIT" << std::endl;
	game_step = GAME_STEP::CREATE_MAP;
}
void GameMaster::CreateMap()
{
	std::cout << "GAME MAP" << std::endl;
	game_step = GAME_STEP::TURN_START;
	
	/* 階層移動の場合は, マップを破棄して乱数を+1する. */
	if (game_map) {
		delete game_map;
		cnt_seed++;
	}
	/* 階層を進む */
	floor_number++;
	/* 部屋数を決定 */
	room_number = floor_number;
	/* 基礎情報登録 + マップ生成 */
	game_map = new RougeLikeMap<MAP_TYPE>(random_seed+cnt_seed);
	game_map->Init(width,height,room_number);
	game_map->SetBaseInfo(static_cast<MAP_TYPE>(MAPSET::DATA::NONE), static_cast<MAP_TYPE>(MAPSET::DATA::ROOM), static_cast<MAP_TYPE>(MAPSET::DATA::ROAD), static_cast<MAP_TYPE>(MAPSET::DATA::WALL));
	game_map->Generate();

	/* 各レイヤを結合 */
	game_map->Update();
}
void GameMaster::TurnStart()
{
	std::cout << "GAME START" << std::endl;
	game_step = GAME_STEP::PLAYER_TURN;
	/* ターンを経過 */
	turn_number++;
}
void GameMaster::PlayerTurn()
{
	std::cout << "GAME PLAYER" << std::endl;
	game_step = GAME_STEP::ITEM_TURN;
}

/* 描画処理 */
void GameMaster::DrawMap()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GameMaster::DrawStatus()
{
}