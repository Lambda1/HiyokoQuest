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
	if (player) { delete player; player = nullptr; }
	if (stair) { delete stair; stair = nullptr; }
	if (game_map) { delete game_map; game_map = nullptr;}
	DiposeEnemy();
}

/* ゲーム更新処理 */
void GameMaster::Update()
{
	/* ターン処理 */
	TurnProcess();
	/* ゲーム描画 */
	if (!game_over_flag && game_step >= GAME_STEP::TURN_START)
	{
		DrawAll();
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
	case GAME_STEP::DISPLAY_INFO:
		DispInfo(); break;
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
	case GAME_STEP::GAME_END:
		GameEnd(); break;
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
	draw_manager.SetSize(width, height);
}
/* マップ生成処理 */
void GameMaster::CreateMap()
{
	std::cout << "GAME MAP" << std::endl;
	
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
	for (int i = 0; i < floor_number; i++) {
		Enemy *enemy_tmp = new Enemy;
		enemy_list.push_back(enemy_tmp);
	}

	/* アイテム生成 */

	/* プレイヤー配置 */
	int player_pos_x, player_pos_y;
	game_map->GetRoomPos(&player_pos_x, &player_pos_y); /* ランダムに部屋の座標を取得 */
	game_map->SetChara(player_pos_y, player_pos_x,static_cast<MAP_TYPE>(player->GetCharaInfo())); /* マップにプレイヤーを配置 */
	player->InitPos(static_cast<POS_TYPE>(player_pos_x), static_cast<POS_TYPE>(player_pos_y)); /* プレイヤー座標を初期化  */

	/* エネミー配置 */
	for (std::list<Character*>::iterator itr = enemy_list.begin(); itr != enemy_list.end(); itr++) {
		int enemy_pos_x, enemy_pos_y;
		game_map->GetRoomPos(&enemy_pos_x, &enemy_pos_y);
		game_map->SetChara(enemy_pos_y, enemy_pos_x, static_cast<MAP_TYPE>((*itr)->GetCharaInfo())); /* マップに敵を配置 */
		(*itr)->InitPos(static_cast<POS_TYPE>(enemy_pos_x), static_cast<POS_TYPE>(enemy_pos_y)); /* 敵座標を初期化 */
	}

	/* アイテム配置 */

	/* 各レイヤを結合 */
	game_map->Update();

	game_step = GAME_STEP::DISPLAY_INFO;
}

/* ダンジョン情報表示 */
void GameMaster::DispInfo()
{
	std::cout << "DISPLAY INFO" << std::endl;
	draw_manager.DrawInit();
	if (draw_manager.DrawBlackScreen(floor_number, 100))
	{
		game_step = GAME_STEP::TURN_START;
	}
}

/* ターン開始処理 */
void GameMaster::TurnStart()
{
	std::cout << "GAME START " << turn_number << std::endl;
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
				if (CharacterMove(player, static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR))) { player->SetTurnMode(TURN_MODE::MOVE); }
			}
			/* プレイヤー攻撃処理 (ABXY入力有り) */
			if (((key_pos & BUTTON_MASK::ABXY) == BUTTON_MASK::ATTACK)) {
				if (CharacterAttack(player)) { player->SetTurnMode(TURN_MODE::ATTACK); }
			}
		}
	}
	
	/* プレイヤーが行動した場合, ターンを移行 */
	if (player->GetTurnMode() != TURN_MODE::NONE)
	{
		game_step = GAME_STEP::STAIR_TURN;
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
		player->SetTurnMode(TURN_MODE::NONE);
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

	bool is_next_turn = true;

	for (std::list<Character*>::iterator itr = enemy_list.begin(); itr != enemy_list.end(); itr++)
	{
		(*itr)->Update();
		const bool is_death = EnemyDeath(itr); /* 敵が倒されている場合, 処理: 死亡+経験値 */
		
		/* 生存している場合の処理 */
		if (!is_death)
		{
			if ((*itr)->GetTurnMode() == TURN_MODE::NONE)
			{
				/* ランダムAI */
				if (GetDirectionInfo((*itr)->GetPosX(),(*itr)->GetPosY(), (*itr)->GetDirect()) == MAPSET::DATA::PLAYER || GetDirectionInfo((*itr)->GetPosX(), (*itr)->GetPosY(), (*itr)->GetDirect()) == MAPSET::DATA::ENEMY)
				{
					if (CharacterAttack(*itr)) { (*itr)->SetTurnMode(TURN_MODE::ATTACK); }
				}
				else {
					DIRECTION tester = static_cast<DIRECTION>(1 << rand() % 4);
					if (CharacterMove(*itr, tester)) { (*itr)->SetTurnMode(TURN_MODE::MOVE); }
					else { (*itr)->SetTurnMode(TURN_MODE::END); }
				}
			}

			if ((*itr)->GetTurnMode() == TURN_MODE::NONE) { is_next_turn = false; }
		}
	}

	/* 全ての敵が行動した場合, ターンを移行 */
	if (is_next_turn) { game_step = GAME_STEP::STATUS_TURN; }
}
/* ステータスターン処理 */
void GameMaster::StatusTurn()
{
	std::cout << "GAME STATUS" << std::endl;
	player->Update(); /* 敵からの攻撃による死亡判定を行うため, ステータス処理時に更新 */
	if (player->IsDeath()) { game_step = GAME_STEP::GAME_END; }
	else { game_step = GAME_STEP::TURN_END; }
}
/* ターン終了処理 */
void GameMaster::TurnEnd()
{
	std::cout << "TURN END" << std::endl;
	/* アニメーションを描画 */
	/* アニメーション終了後に, 初めのターンに戻る */
	if(AnimationUpdate()) { game_step = GAME_STEP::TURN_START; }
}
/* ゲーム終了処理 */
void GameMaster::GameEnd()
{
	std::cout << "GAME END" << std::endl;
	DiposeFloor();
	if (player) { delete player; player = nullptr; }
	game_over_flag = true;
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
MAPSET::DATA GameMaster::GetDirectionInfo(POS_TYPE x, POS_TYPE y,const DIRECTION direct)
{
	switch (direct)
	{
	case DIRECTION::EAST:
		(x)++; break;
	case DIRECTION::WEST:
		(x)--; break;
	case DIRECTION::SOUTH:
		(y)++; break;
	case DIRECTION::NORTH:
		(y)--; break;
	case DIRECTION::SOUTH_EAST:
		(x)++; (y)++; break;
	case DIRECTION::SOUTH_WEST:
		(x)--; (y)++; break;
	case DIRECTION::NORTH_EAST:
		(x)++; (y)--; break;
	case DIRECTION::NORTH_WEST:
		(x)--; (y)--; break;
	default:
		break;
	}
	return static_cast<MAPSET::DATA>(game_map->GetALL()[static_cast<int>(y) * width + static_cast<int>(x)]);
}
bool GameMaster::IsPosMove(const int x, const int y)
{
	MAPSET::DATA dungeon_data = static_cast<MAPSET::DATA>(game_map->GetLayerALL(x, y));
	MAPSET::DATA character_layer = static_cast<MAPSET::DATA>(game_map->GetLayerChara(x, y));

	/* 道 or 部屋 or 階段ならば, 移動可能 */
	if (dungeon_data == MAPSET::DATA::ROAD || dungeon_data == MAPSET::DATA::ROOM || dungeon_data == MAPSET::DATA::STAIR)
	{
		/* 敵の座標更新時にレイヤ結合を行っていないため, キャラレイヤの座標確認も行う */
		if (character_layer == MAPSET::DATA::NONE || character_layer == MAPSET::DATA::STAIR)
		{
			return true;
		}
	}
	return false;
}
MAPSET::DATA GameMaster::IsPosAttack(const int& x, const int& y)
{
	return static_cast<MAPSET::DATA>(game_map->GetChara()[y*width + x]);
}
/* キャラクタ汎用移動処理 */
bool GameMaster::CharacterMove(Character *ch_data, const DIRECTION& direct)
{
	/* キャラクタが進行方向に移動可能か判定 */
	POS_TYPE pos_x = ch_data->GetPosX(), pos_y = ch_data->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, direct);
	ch_data->SetDirection(direct); /* キャラクタの向きを入力方向へ */
	/* 移動可能の場合, キャラクタを移動する */
	if (IsPosMove(static_cast<int>(pos_x), static_cast<int>(pos_y)))
	{
		game_map->SetChara(static_cast<int>(ch_data->GetPosY()), static_cast<int>(ch_data->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* 現在地点をクリア */
		ch_data->Move(direct);
		game_map->SetChara(static_cast<int>(pos_y), static_cast<int>(pos_x), static_cast<MAP_TYPE>(ch_data->GetCharaInfo())); /* 移動地点にキャラクタを配置 */
		return true;
	}
	return false;
}
/* 汎用攻撃処理 */
bool GameMaster::CharacterAttack(Character* ch_data)
{
	POS_TYPE pos_x = ch_data->GetPosX(), pos_y = ch_data->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, ch_data->GetDirect());
	
	/* 攻撃先に主人公が存在する場合, 攻撃計算処理 */
	if (IsPosAttack(static_cast<int>(pos_x), static_cast<int>(pos_y)) == MAPSET::DATA::PLAYER)
	{
		int damage = ch_data->Attack(player->GetDefence());
		player->Damaged(damage);
		player->SetAttacked(ch_data->GetCharaInfo());
	}
	/* 攻撃先に敵が存在する場合, 攻撃計算処理 */
	else if (IsPosAttack(static_cast<int>(pos_x), static_cast<int>(pos_y)) == MAPSET::DATA::ENEMY)
	{
		/* 敵を全探索して, 計算処理 */
		for (std::list<Character*>::iterator itr = enemy_list.begin(); itr != enemy_list.end(); itr++)
		{
			if ((*itr)->GetPosX() == pos_x && (*itr)->GetPosY() == pos_y)
			{
				int damege = ch_data->Attack((*itr)->GetDefence());
				(*itr)->Damaged(damege);
				(*itr)->SetAttacked(ch_data->GetCharaInfo());
				break;
			}
		}
	}
	return true;
}
/* アニメーション処理 */
bool GameMaster::AnimationUpdate()
{
	/* 主人公: アニメーション */
	if (player->GetTurnMode() == TURN_MODE::MOVE)
	{
		player->MoveAnimation();
		if (player->GetPosX() == player->GetPosPX() && player->GetPosY() == player->GetPosPY()) { player->SetTurnMode(TURN_MODE::END); }
	}
	else if (player->GetTurnMode() == TURN_MODE::ATTACK)
	{
		player->AttackAnimation();
	}
	if (player->GetTurnMode() == TURN_MODE::END)
	{
		player->SetTurnMode(TURN_MODE::NONE);
	}

	/* 敵: アニメーション */
	for (std::list<Character*>::iterator itr = enemy_list.begin(); itr != enemy_list.end(); itr++)
	{
		if ((*itr)->GetTurnMode() == TURN_MODE::MOVE)
		{
			(*itr)->MoveAnimation();
			if ((*itr)->GetPosX() == (*itr)->GetPosPX() && (*itr)->GetPosY() == (*itr)->GetPosPY()) { (*itr)->SetTurnMode(TURN_MODE::END); }
		}
		else if ((*itr)->GetTurnMode() == TURN_MODE::ATTACK) { (*itr)->AttackAnimation(); }

		if ((*itr)->GetTurnMode() == TURN_MODE::END) { (*itr)->SetTurnMode(TURN_MODE::NONE); }
	}

	/* 全てのキャラクタのアニメーションは終了しているか? */
	if (player->GetTurnMode() != TURN_MODE::NONE) return false;
	for (std::list<Character*>::iterator itr = enemy_list.begin(); itr != enemy_list.end(); itr++) { if ((*itr)->GetTurnMode() != TURN_MODE::NONE) return false; }
	return true;
}

/* StairTurn専用処理 */
/* 次回層に進むための処理*/
/* 乱数更新, メモリ解放処理 */
void GameMaster::DiposeFloor()
{
	cnt_seed++;
	DiposeEnemy();
	if (game_map) { delete game_map; game_map = nullptr; }
	if (stair) { delete stair; stair = nullptr; }
}
void GameMaster::DiposeEnemy()
{
	for (std::list<Character*>::iterator itr = enemy_list.begin(); itr != enemy_list.end(); itr++) {
		if (*itr)
		{
			game_map->SetChara(static_cast<int>((*itr)->GetPosY()), static_cast<int>((*itr)->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* 現在地点をクリア */
			delete *itr;
			enemy_list.erase(itr);
		}
	}
}

/* EnemyTurn専用処理 */
/* Enemyの死亡処理 */
bool GameMaster::EnemyDeath(const std::list<Character*>::iterator& enemy_itr)
{
	if ((*enemy_itr)->IsDeath())
	{
		/* プレイヤーに倒された場合, プレイヤーに経験値処理 */
		if ((*enemy_itr)->GetAttacked() == MAPSET::DATA::PLAYER)
		{
			player->GetEXP((*enemy_itr)->GiveEXP());
			player->Update();
		}

		game_map->SetChara(static_cast<int>((*enemy_itr)->GetPosY()), static_cast<int>((*enemy_itr)->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* 現在地点をクリア */
		delete* enemy_itr;
		enemy_list.erase(enemy_itr);
		return true;
	}
	return false;
}

/* 描画処理 */
void GameMaster::CameraPos()
{
	draw_manager.CameraPos(player->GetPosPX(), 10.0f, player->GetPosPY() + 10.0f, player->GetPosPX(), 0.0f, player->GetPosPY());
}
void GameMaster::DrawMap()
{
	draw_manager.DrawInit(); /* 画面クリア */

	draw_manager.DrawMap(game_map->GetDungeon(), width, height, static_cast<int>(player->GetPosX()), static_cast<int>(player->GetPosY())); /* マップ表示 */
	/* アイテム表示 */
	draw_manager.DrawCharacter(player); /* 主人公表示 */
	draw_manager.DrawCharacter(stair, width, height, static_cast<int>(player->GetPosX()), static_cast<int>(player->GetPosY()));  /* 階段表示 */

	for (std::list<Character*>::iterator itr = enemy_list.begin(); itr != enemy_list.end(); itr++) {
		draw_manager.DrawCharacter((*itr), width, height, static_cast<int>((*itr)->GetPosX()), static_cast<int>((*itr)->GetPosY()));  /* 敵表示 */
	}
}
void GameMaster::DrawStatus()
{
	draw_manager.DrawStatusBar(player, floor_number);
}
void GameMaster::DrawAll()
{
	game_map->Update(); /* マップ更新 */
	CameraPos();  /* カメラ設定 */
	DrawMap();    /* マップ描画 */
	DrawStatus(); /* ステータス描画 */
}