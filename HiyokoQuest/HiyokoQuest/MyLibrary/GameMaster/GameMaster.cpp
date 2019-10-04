#include "./GameMaster.hpp"

GameMaster::GameMaster() :
	game_step(GAME_STEP::INIT),
	game_over_flag(false), turn_cost_flag(false),
	key_pos(BUTTON_MASK::NONE), all_key_release(false),
	random_seed(static_cast<int>(std::time(nullptr))), cnt_seed(0),
	game_map(nullptr) , floor_number(0), turn_number(0), room_number(0),
	player(nullptr)
{
}
GameMaster::~GameMaster()
{
	if (player) { delete player; }
	if (game_map) { delete game_map; }
}

/* ゲーム更新処理 */
void GameMaster::Update()
{
	/* ターン処理 */
	TurnProcess();

	/* ゲーム描画 */
	if (game_step >= GAME_STEP::TURN_START)
	{
		DrawMap();
		DrawStatus();
	}
}

void GameMaster::KeyInput(const bool* key_on,const bool* key_prev)
{
	key_pos = BUTTON_MASK::NONE;
	/* 最大入力: 2 */
	int key_input_number = 0;
	for (int i = 0; i < ALPHABET && key_input_number < MAX_KEY_INPUT; i++) {
		if (key_on[i])
		{
			key_input_number++;
			switch (static_cast<BUTTON>(KEY_BIAS+i))
			{
			case BUTTON::UP:
				key_pos = key_pos | BUTTON_MASK::UP; break;
			case BUTTON::DOWN:
				key_pos = key_pos | BUTTON_MASK::DOWN; break;
			case BUTTON::RIGHT:
				key_pos = key_pos | BUTTON_MASK::RIGHT; break;
			case BUTTON::LEFT:
				key_pos = key_pos | BUTTON_MASK::LEFT; break;
			case BUTTON::OK:
				key_pos = key_pos | BUTTON_MASK::OK; break;
			case BUTTON::CANCEL:
				key_pos = key_pos | BUTTON_MASK::CANCEL; break;
			default:
				key_input_number--; break;
			}
		}
	}
}

/* private */
/* ゲーム処理 */
/* ターン統括処理 */
void GameMaster::TurnProcess()
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
		ItemTurn();   break;
	case GAME_STEP::ENEMY_TURN:
		break;
	case GAME_STEP::STATUS_TURN:
		break;
	case GAME_STEP::TURN_END:
		break;
	default:
		break;
	}
}
/* 初期化処理 */
void GameMaster::Init()
{
	std::cout << "GAME INIT" << std::endl;
	game_step = GAME_STEP::CREATE_MAP;

	/* プレイヤー招来 */
	player = new Player;
	/* エネミー召喚 */

	/* アイテム生成 */

}
/* マップ生成処理 */
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

	/* プレイヤー配置 */
	int pos_x, pos_y;
	game_map->GetRoomPos(&pos_x, &pos_y); /* ランダムに部屋の座標を取得 */
	game_map->SetChara(pos_y, pos_x,static_cast<MAP_TYPE>(player->GetCharaInfo())); /* マップにプレイヤーを配置 */
	player->InitPos(pos_x,pos_y); /* プレイヤー座標を初期化  */

	/* エネミー配置 */

	/* アイテム配置 */

	/* 各レイヤを結合 */
	game_map->Update();

	MAP_TYPE* test = game_map->GetALL();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) std::cout << (int)test[i * width + j];
		std::cout << std::endl;
	}
}
/* ターン開始処理 */
void GameMaster::TurnStart()
{
	std::cout << "GAME START" << std::endl;
	game_step = GAME_STEP::PLAYER_TURN;
	/* ターンを経過 */
	turn_number++;
}
/* プレイヤーターン処理 */
void GameMaster::PlayerTurn()
{
	std::cout << "GAME PLAYER" << std::endl;

	/* キー入力があった場合, 処理を開始 */
	if (key_pos != BUTTON_MASK::NONE)
	{
		/* プレイヤー移動処理 */
		if (!turn_cost_flag) {
			turn_cost_flag = PlayerMove();
		}
		/* プレイヤー攻撃処理 */
		if (!turn_cost_flag) {
			turn_cost_flag = PlayerAttack();
		}
		/* 行動を消費していたら, アイテムターンへ移行 */
		if (turn_cost_flag)
		{
			player->Update(); /* プレイヤー情報更新 */
			turn_cost_flag = false;
			game_step = GAME_STEP::ITEM_TURN;
		}
	}
}
/* アイテムターン処理 */
void GameMaster::ItemTurn()
{
	std::cout << "GAME ITEM" << std::endl;
	game_step = GAME_STEP::TURN_START;
}

/* キャラクター処理 */
void GameMaster::CalcDirectionToPos(int *x, int *y, DIRECTION direct)
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
bool GameMaster::IsPosMove(const int x, const int y)
{
	MAPSET::DATA dungeon_data = static_cast<MAPSET::DATA>(game_map->GetLayerDungeon(x,y));

	/* 道 and 部屋ならば, 移動可能 */
	if (dungeon_data == MAPSET::DATA::ROAD || dungeon_data == MAPSET::DATA::ROOM)
	{
		return true;
	}

	return false;
}

/* PlayerTurn専用処理 */
/* プレイヤー移動処理 */
bool GameMaster::PlayerMove()
{
	/* プレイヤーが進行方向に移動可能か判定 */
	int pos_x = player->GetPosX(), pos_y = player->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR));
	/* 移動可能の場合, プレイヤーを移動する */
	if (IsPosMove(pos_x, pos_y)) {
		game_map->SetChara(player->GetPosY(), player->GetPosX(), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* 現在地点をクリア */
		player->Move(static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR)); /* 移動する */
		game_map->SetChara(pos_y, pos_x, static_cast<MAP_TYPE>(player->GetCharaInfo())); /* 移動地点にplayerを配置 */
		return true;
	}
	return false;
}
/* プレイヤー攻撃処理 */
bool GameMaster::PlayerAttack()
{
	return false;
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