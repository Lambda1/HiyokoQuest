#include "./GameMaster.hpp"

GameMaster::GameMaster() :
	game_step(GAME_STEP::INIT),
	game_over_flag(false),
	key_pos(BUTTON_MASK::NONE), all_key_release(false),
	random_seed(static_cast<int>(std::time(nullptr))), cnt_seed(0),
	game_map(nullptr) , floor_number(0), turn_number(0), room_number(0),
	player(nullptr), stair(nullptr)
{
}
GameMaster::~GameMaster()
{
	if (player) { delete player; }
	if (stair) { delete stair; }
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
		CameraPos();  /* カメラ設定 */
		DrawMap();    /* マップ描画 */
		DrawStatus(); /* ステータス描画 */
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
			case BUTTON::ATTACK:
				key_pos = key_pos | BUTTON_MASK::ATTACK; break;
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
	case GAME_STEP::STAIR_TURN:
		StairTurn(); break;
	case GAME_STEP::ITEM_TURN:
		ItemTurn();   break;
	case GAME_STEP::ENEMY_TURN:
		EnemyTurn(); break;
	case GAME_STEP::STATUS_TURN:
		StatusTurn(); break;
	case GAME_STEP::TURN_END:
		TurnEnd(); break;
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

	/* 描画処理初期化 */
	draw_manager.Init();
}
/* マップ生成処理 */
void GameMaster::CreateMap()
{
	std::cout << "GAME MAP" << std::endl;
	game_step = GAME_STEP::TURN_START;
	
	/* 階層を進む */
	floor_number++;
	/* 部屋数を決定 */
	room_number = floor_number;
	/* 基礎情報登録 + マップ生成 */
	game_map = new RougeLikeMap<MAP_TYPE>(random_seed+cnt_seed);
	game_map->Init(width,height,room_number);
	game_map->SetBaseInfo(static_cast<MAP_TYPE>(MAPSET::DATA::NONE), static_cast<MAP_TYPE>(MAPSET::DATA::ROOM), static_cast<MAP_TYPE>(MAPSET::DATA::ROAD), static_cast<MAP_TYPE>(MAPSET::DATA::WALL));
	game_map->Generate();

	/* 階段生成 */
	stair = new Stair;
	int stair_pos_x, stair_pos_y;
	game_map->GetRoomPos(&stair_pos_x, &stair_pos_y);
	game_map->SetChara(stair_pos_y, stair_pos_x, static_cast<MAP_TYPE>(stair->GetCharaInfo())); /* マップに階段を配置 */
	stair->InitPos(static_cast<POS_TYPE>(stair_pos_x), static_cast<POS_TYPE>(stair_pos_y)); /* 階段座標を初期化 */
	/* エネミー召喚 */

	/* アイテム生成 */

	/* プレイヤー配置 */
	int player_pos_x, player_pos_y;
	game_map->GetRoomPos(&player_pos_x, &player_pos_y); /* ランダムに部屋の座標を取得 */
	game_map->SetChara(player_pos_y, player_pos_x,static_cast<MAP_TYPE>(player->GetCharaInfo())); /* マップにプレイヤーを配置 */
	player->InitPos(static_cast<POS_TYPE>(player_pos_x), static_cast<POS_TYPE>(player_pos_y)); /* プレイヤー座標を初期化  */

	/* エネミー配置 */

	/* アイテム配置 */

	/* 各レイヤを結合 */
	game_map->Update();
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
	//std::cout << "GAME PLAYER" << std::endl;

	/* キー入力があった場合, 処理を開始 */
	if (key_pos != BUTTON_MASK::NONE)
	{
		/* 行動を消費していない場合の処理 */
		if (player->GetTurnMode() == TURN_MODE::NONE)
		{
			/* プレイヤー移動処理 (十字入力有り) */
			if (((key_pos & BUTTON_MASK::CURSOR) != BUTTON_MASK::NONE)) {
				/* 移動可能ならば, マップを移動して移動処理に移行 */
				if (PlayerMove()) { player->SetTurnMode(TURN_MODE::MOVE); }
			}
			/* プレイヤー攻撃処理 (ABXY入力有り) */
			if (((key_pos & BUTTON_MASK::ABXY) != BUTTON_MASK::NONE)) {
				if (PlayerAttack()) { player->SetTurnMode(TURN_MODE::ATTACK); }
			}
		}
	}
	/* 描画用の処理 */
	if (player->GetTurnMode() == TURN_MODE::MOVE)
	{
		player->MoveAnimation();
	}
	/* 行動を消費していたら, 次ターンへ移行 */
	else if (player->GetTurnMode() == TURN_MODE::END)
	{
		player->Update(); /* プレイヤー情報更新 */
		player->SetTurnMode(TURN_MODE::NONE);
		game_step = GAME_STEP::STAIR_TURN;
		
		game_map->Update();
	}
}
/* 階層ターン処理 */
void GameMaster::StairTurn()
{
	std::cout << "GAME STAIR" << std::endl;
	
	/* PlayerとSTAIRの座標が等しい時, 次階層へ移動 */
	if (*player == *stair)
	{
		DiposeFloor();
		game_step = GAME_STEP::CREATE_MAP;
	}
	else { game_step = GAME_STEP::ITEM_TURN; }
}
/* アイテムターン処理 */
void GameMaster::ItemTurn()
{
	std::cout << "GAME ITEM" << std::endl;
	game_step = GAME_STEP::ENEMY_TURN;
}
/* エネミーターン処理 */
void GameMaster::EnemyTurn()
{
	std::cout << "GAME ENEMY" << std::endl;
	game_step = GAME_STEP::STATUS_TURN;
}
/* ステータスターン処理 */
void GameMaster::StatusTurn()
{
	std::cout << "GAME STATUS" << std::endl;
	game_step = GAME_STEP::TURN_END;
}
/* ターン終了処理 */
void GameMaster::TurnEnd()
{
	std::cout << "GAME END" << std::endl;
	game_step = GAME_STEP::TURN_START;
}

/* キャラクター処理 */
void GameMaster::CalcDirectionToPos(POS_TYPE *x, POS_TYPE *y, DIRECTION direct)
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
	POS_TYPE pos_x = player->GetPosX(), pos_y = player->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR));
	/* 移動可能の場合, プレイヤーを移動する */
	if (IsPosMove(static_cast<int>(pos_x), static_cast<int>(pos_y)))
	{
		game_map->SetChara(static_cast<int>(player->GetPosY()), static_cast<int>(player->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* 現在地点をクリア */
		player->Move(static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR)); /* 移動する */
		game_map->SetChara(static_cast<int>(pos_y), static_cast<int>(pos_x), static_cast<MAP_TYPE>(player->GetCharaInfo())); /* 移動地点にplayerを配置 */
		return true;
	}
	return false;
}
/* プレイヤー攻撃処理 */
bool GameMaster::PlayerAttack()
{
	return false;
}

/* StairTurn専用処理 */
/* 次回層に進むための処理*/
/* 乱数更新, メモリ解放処理 */
void GameMaster::DiposeFloor()
{
	cnt_seed++;
	if (game_map) delete game_map;
	if (stair) delete stair;
}

/* 描画処理 */
void GameMaster::CameraPos()
{
	draw_manager.CameraPos(player->GetPosPX(), 10.0f, player->GetPosPY() + 10.0f, player->GetPosPX(), 0.0f, player->GetPosPY());
}
void GameMaster::DrawMap()
{
	draw_manager.DrawInit(); /* 画面クリア */

	draw_manager.DrawMap(game_map->GetALL(), width, height, static_cast<int>(player->GetPosX()), static_cast<int>(player->GetPosY())); /* マップ表示 */
	/* アイテム表示 */
	draw_manager.DrawCharacter(player); /* 主人公表示 */
}
void GameMaster::DrawStatus()
{
}