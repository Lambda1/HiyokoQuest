#include "./GameMaster.hpp"

GameMaster::GameMaster() :
	game_step(GAME_STEP::INIT),
	game_over_flag(false),
	key_pos(BUTTON_MASK::NONE), all_key_release(false),
	random_seed(static_cast<int>(std::time(nullptr))), cnt_seed(0),
	game_map(nullptr) , floor_number(0), turn_number(0), room_number(0),
	player(nullptr), stair(nullptr)
{
	InitTurnProcessMap();
}
GameMaster::~GameMaster()
{
	DiposeEnemy();
	if (player) { delete player; player = nullptr; }
	if (stair) { delete stair; stair = nullptr; }
	if (game_map) { delete game_map; game_map = nullptr;}
}

/* �Q�[���X�V���� */
void GameMaster::Update()
{
	/* �^�[������ */
	TurnProcess();
	/* �Q�[���`�� */
	if (!game_over_flag && game_step >= GAME_STEP::TURN_START)
	{
		DrawAll();
	}
}

void GameMaster::KeyInput(const bool* key_on,const bool* key_prev)
{
	key_pos = BUTTON_MASK::NONE;
	/* �ő����: 2 */
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
/* �Q�[������ */
/* �^�[���������� */
void GameMaster::TurnProcess()
{
	if (manage_turn_process.find(game_step) != manage_turn_process.end()) { (this->*manage_turn_process[game_step])(); }
}
/* ���������� */
void GameMaster::Init()
{
	game_step = GAME_STEP::CREATE_MAP;

	/* �v���C���[���� */
	player = new Player;

	/* �`�揈�������� */
	draw_manager.Init();
	draw_manager.SetSize(width, height);
}
/* �}�b�v�������� */
void GameMaster::CreateMap()
{
	/* �K�w��i�� */
	floor_number++;
	/* ������������ */
	room_number = floor_number;
	/* ��b���o�^ + �}�b�v���� */
	game_map = new RougeLikeMap<MAP_TYPE>(random_seed+cnt_seed);
	game_map->Init(width,height,room_number);
	game_map->SetBaseInfo(static_cast<MAP_TYPE>(MAPSET::DATA::NONE), static_cast<MAP_TYPE>(MAPSET::DATA::ROOM), static_cast<MAP_TYPE>(MAPSET::DATA::ROAD), static_cast<MAP_TYPE>(MAPSET::DATA::WALL));
	game_map->Generate();

	/* �K�i���� */
	stair = new Stair;
	int stair_pos_x, stair_pos_y;
	game_map->GetRoomPos(&stair_pos_x, &stair_pos_y);
	game_map->SetField(stair_pos_y, stair_pos_x, static_cast<MAP_TYPE>(stair->GetCharaInfo())); /* �}�b�v�ɊK�i��z�u */
	stair->InitPos(static_cast<POS_TYPE>(stair_pos_x), static_cast<POS_TYPE>(stair_pos_y)); /* �K�i���W�������� */
	
	/* �G�l�~�[���� */
	int random_index = 0;
	for (int i = 0; i < floor_number*2; i++) {
		Enemy *enemy_tmp = new Enemy(static_cast<float>(floor_number*0.5f),static_cast<MAPSET::DATA>((int)MAPSET::DATA::ENEMY1+random_index));
		enemy_list.push_back(enemy_tmp);
		random_index = (random_index+1) % ((int)MAPSET::DATA::ENEMY5 - (int)MAPSET::DATA::ENEMY1);
	}

	/* �A�C�e������ */

	/* �v���C���[�z�u */
	int player_pos_x, player_pos_y;
	game_map->GetRoomPos(&player_pos_x, &player_pos_y); /* �����_���ɕ����̍��W���擾 */
	game_map->SetChara(player_pos_y, player_pos_x,static_cast<MAP_TYPE>(player->GetCharaInfo())); /* �}�b�v�Ƀv���C���[��z�u */
	player->InitPos(static_cast<POS_TYPE>(player_pos_x), static_cast<POS_TYPE>(player_pos_y)); /* �v���C���[���W��������  */

	/* �G�l�~�[�z�u */
	for (auto itr = enemy_list.begin(); itr != enemy_list.end(); itr++) {
		int enemy_pos_x, enemy_pos_y;
		game_map->GetRoomPos(&enemy_pos_x, &enemy_pos_y);
		game_map->SetChara(enemy_pos_y, enemy_pos_x, static_cast<MAP_TYPE>((*itr)->GetCharaInfo())); /* �}�b�v�ɓG��z�u */
		(*itr)->InitPos(static_cast<POS_TYPE>(enemy_pos_x), static_cast<POS_TYPE>(enemy_pos_y)); /* �G���W�������� */
	}

	/* �A�C�e���z�u */

	/* �e���C�������� */
	game_map->Update();

	game_step = GAME_STEP::DISPLAY_INFO;
}

/* �_���W�������\�� */
void GameMaster::DispInfo()
{
	draw_manager.DrawInit();
	if (draw_manager.DrawBlackScreen(floor_number, static_cast<int>(BASE_FPS*1.5f))) { game_step = GAME_STEP::TURN_START; }
}

/* �^�[���J�n���� */
void GameMaster::TurnStart()
{
	game_step = GAME_STEP::PLAYER_TURN;
	/* �^�[�����o�� */
	turn_number++;
	/* �~�j�}�b�v�̍X�V */
	game_map->MiniMapUpdate(static_cast<int>(player->GetPosX()), static_cast<int>(player->GetPosY()));
}
/* �v���C���[�^�[������ */
void GameMaster::PlayerTurn()
{
	/* �L�[���͂��������ꍇ, �������J�n */
	if (key_pos != BUTTON_MASK::NONE)
	{
		/* �s��������Ă��Ȃ��ꍇ�̏��� */
		if (player->GetTurnMode() == TURN_MODE::NONE)
		{
			/* �v���C���[�ړ����� (�\�����͗L��) */
			if (((key_pos & BUTTON_MASK::CURSOR) != BUTTON_MASK::NONE)) {
				/* �ړ��\�Ȃ��, �}�b�v���ړ����Ĉړ������Ɉڍs */
				if (CharacterMove(player, static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR))) { player->SetTurnMode(TURN_MODE::MOVE); }
			}
			/* �v���C���[�U������ (ABXY���͗L��) */
			else if (((key_pos & BUTTON_MASK::ABXY) == BUTTON_MASK::ATTACK)) {
				if (CharacterAttack(player)) { player->SetTurnMode(TURN_MODE::ATTACK); }
			}
		}
	}
	
	/* �v���C���[���s�������ꍇ, �^�[�����ڍs */
	if (player->GetTurnMode() != TURN_MODE::NONE)
	{
		game_step = GAME_STEP::ENEMY_TURN;
	}
}
/* �A�C�e���^�[������ */
void GameMaster::ItemTurn()
{
	game_step = GAME_STEP::ENEMY_TURN;
}
/* �G�l�~�[�^�[������ */
void GameMaster::EnemyTurn()
{
	bool is_next_turn = true;

	/* iterator�̔j��ɒ��ӂ��邱�� */
	for (auto itr = enemy_list.begin(); itr != enemy_list.end();)
	{
		(*itr)->Update();
		const bool is_death = EnemyDeath(itr); /* �G���|����Ă���ꍇ, ����: ���S+�o���l */
		
		/* �������Ă���ꍇ�̏��� */
		if (!is_death)
		{
			if ((*itr)->GetTurnMode() == TURN_MODE::NONE)
			{
				/* �����_��AI */
				/*
				if (GetDirectionInfo((*itr)->GetPosX(),(*itr)->GetPosY(), (*itr)->GetDirect()) == MAPSET::DATA::PLAYER || GetDirectionInfo((*itr)->GetPosX(), (*itr)->GetPosY(), (*itr)->GetDirect()) == MAPSET::DATA::ENEMY)
				{
					if (CharacterAttack(*itr)) { (*itr)->SetTurnMode(TURN_MODE::ATTACK); }
				}
				else {
					DIRECTION tester = static_cast<DIRECTION>(1 << rand() % 4);
					if (CharacterMove(*itr, tester)) { (*itr)->SetTurnMode(TURN_MODE::MOVE); }
					else { (*itr)->SetTurnMode(TURN_MODE::END); }
				}
				*/
				DIRECTION direct = (*itr)->AI_Move(game_map->GetALL(),width,height);
				CharacterMove(*itr,direct);
			}

			if ((*itr)->GetTurnMode() == TURN_MODE::NONE) { is_next_turn = false; }
		}
		else {
			continue;
		}
		itr++;
	}

	/* �S�Ă̓G���s�������ꍇ, �^�[�����ڍs */
	if (is_next_turn)
	{
		game_step = GAME_STEP::STATUS_TURN;
	}
}
/* �X�e�[�^�X�^�[������ */
void GameMaster::StatusTurn()
{
	player->Update(); /* �G����̍U���ɂ�鎀�S������s������, �X�e�[�^�X�������ɍX�V */
	if (player->IsDeath()) { game_step = GAME_STEP::GAME_END; }
	else { game_step = GAME_STEP::TURN_END; }
}
/* �^�[���I������ */
void GameMaster::TurnEnd()
{
	/* �A�j���[�V������`�� */
	/* �A�j���[�V�����I�����, ���^�[���� */
	if (AnimationUpdate()) { game_step = GAME_STEP::STAIR_TURN; }
}
/* �K�w�^�[������ */
void GameMaster::StairTurn()
{
	/* Player��STAIR�̍��W����������, ���K�w�ֈړ� */
	if (*player == *stair)
	{
		DiposeFloor();
		player->SetTurnMode(TURN_MODE::NONE);
		game_step = GAME_STEP::CREATE_MAP;
	}
	else { game_step = GAME_STEP::TURN_START; }
}
/* �Q�[���I������ */
void GameMaster::GameEnd()
{
	DiposeFloor();
	if (player) { delete player; player = nullptr; }
	game_over_flag = true;
}

/* �L�����N�^�[���� */
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

	/* �� or ���� or �K�i�Ȃ��, �ړ��\ */
	if (dungeon_data == MAPSET::DATA::ROAD || dungeon_data == MAPSET::DATA::ROOM || dungeon_data == MAPSET::DATA::STAIR)
	{
		/* �G�̍��W�X�V���Ƀ��C���������s���Ă��Ȃ�����, �L�������C���̍��W�m�F���s�� */
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
/* �L�����N�^�ėp�ړ����� */
bool GameMaster::CharacterMove(Character *ch_data, const DIRECTION& direct)
{
	/* �L�����N�^���i�s�����Ɉړ��\������ */
	POS_TYPE pos_x = ch_data->GetPosX(), pos_y = ch_data->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, direct);
	ch_data->SetDirection(direct); /* �L�����N�^�̌�������͕����� */
	/* �ړ��\�̏ꍇ, �L�����N�^���ړ����� */
	if (IsPosMove(static_cast<int>(pos_x), static_cast<int>(pos_y)))
	{
		game_map->SetChara(static_cast<int>(ch_data->GetPosY()), static_cast<int>(ch_data->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* ���ݒn�_���N���A */
		ch_data->Move(direct);
		game_map->SetChara(static_cast<int>(pos_y), static_cast<int>(pos_x), static_cast<MAP_TYPE>(ch_data->GetCharaInfo())); /* �ړ��n�_�ɃL�����N�^��z�u */
		return true;
	}
	return false;
}
/* �ėp�U������ */
bool GameMaster::CharacterAttack(Character* ch_data)
{
	POS_TYPE pos_x = ch_data->GetPosX(), pos_y = ch_data->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, ch_data->GetDirect());
	
	/* �U����Ɏ�l�������݂���ꍇ, �U���v�Z���� */
	if (IsPosAttack(static_cast<int>(pos_x), static_cast<int>(pos_y)) == MAPSET::DATA::PLAYER)
	{
		int damage = ch_data->Attack(player->GetDefence());
		player->Damaged(damage);
		player->SetAttacked(ch_data->GetCharaInfo());
	}
	/* �U����ɓG�����݂���ꍇ, �U���v�Z���� */
	else if (IsPosAttack(static_cast<int>(pos_x), static_cast<int>(pos_y)) == MAPSET::DATA::ENEMY)
	{
		/* �G��S�T������, �v�Z���� */
		for (auto itr = enemy_list.begin(); itr != enemy_list.end(); itr++)
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
/* �A�j���[�V�������� */
bool GameMaster::AnimationUpdate()
{
	/* ��l��: �A�j���[�V���� */
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

	/* �G: �A�j���[�V���� */
	for (auto itr = enemy_list.begin(); itr != enemy_list.end(); itr++)
	{
		if ((*itr)->GetTurnMode() == TURN_MODE::MOVE)
		{
			(*itr)->MoveAnimation();
			if ((*itr)->GetPosX() == (*itr)->GetPosPX() && (*itr)->GetPosY() == (*itr)->GetPosPY()) { (*itr)->SetTurnMode(TURN_MODE::END); }
		}
		else if ((*itr)->GetTurnMode() == TURN_MODE::ATTACK) { (*itr)->AttackAnimation(); }

		if ((*itr)->GetTurnMode() == TURN_MODE::END) { (*itr)->SetTurnMode(TURN_MODE::NONE); }
	}

	/* �S�ẴL�����N�^�̃A�j���[�V�����͏I�����Ă��邩? */
	if (player->GetTurnMode() != TURN_MODE::NONE) return false;
	for (auto itr = enemy_list.begin(); itr != enemy_list.end(); itr++) { if ((*itr)->GetTurnMode() != TURN_MODE::NONE) return false; }
	return true;
}

/* StairTurn��p���� */
/* ����w�ɐi�ނ��߂̏���*/
/* �����X�V, ������������� */
void GameMaster::DiposeFloor()
{
	cnt_seed++;
	DiposeEnemy();
	if (game_map) { delete game_map; game_map = nullptr; }
	if (stair) { delete stair; stair = nullptr; }
}
void GameMaster::DiposeEnemy()
{
	/* iterator�̔j��ɒ��ӂ��邱�� */
	for (auto itr = enemy_list.begin(); itr != enemy_list.end();) {
		if (*itr)
		{
			game_map->SetChara(static_cast<int>((*itr)->GetPosY()), static_cast<int>((*itr)->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* ���ݒn�_���N���A */
			delete* itr;
			itr = enemy_list.erase(itr);
			continue;
		}
		itr++;
	}
}

/* EnemyTurn��p���� */
/* Enemy�̎��S���� */
bool GameMaster::EnemyDeath(std::list<Enemy*>::iterator& enemy_itr)
{
	if ((*enemy_itr)->IsDeath())
	{
		/* �v���C���[�ɓ|���ꂽ�ꍇ, �v���C���[�Ɍo���l���� */
		if ((*enemy_itr)->GetAttacked() == MAPSET::DATA::PLAYER)
		{
			player->GetEXP((*enemy_itr)->GiveEXP());
			player->Update();
		}

		game_map->SetChara(static_cast<int>((*enemy_itr)->GetPosY()), static_cast<int>((*enemy_itr)->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* ���ݒn�_���N���A */
		delete* enemy_itr;
		enemy_itr = enemy_list.erase(enemy_itr);
		return true;
	}
	return false;
}

/* �`�揈�� */
void GameMaster::CameraPos()
{
	draw_manager.CameraPos(player->GetPosPX(), 10.0f, player->GetPosPY() + 10.0f, player->GetPosPX(), 0.0f, player->GetPosPY());
	//draw_manager.CameraPos(player->GetPosPX(),50.0f, player->GetPosPY(), player->GetPosPX(), 0.0f, (player->GetPosPY() - 0.5f)); /* �V�󃂁[�h(�A���t�@��) */
	//draw_manager.CameraPos(player->GetPosPX(), 1.0f, player->GetPosPY(), player->GetPosPX(), 0.9f, (player->GetPosPY()-0.5f)); /* FPS���[�h(�A���t�@��) */
}
void GameMaster::DrawMap()
{
	draw_manager.DrawInit(); /* ��ʃN���A */

	draw_manager.DrawMap(game_map->GetDungeon(), width, height, static_cast<int>(player->GetPosX()), static_cast<int>(player->GetPosY())); /* �}�b�v�\�� */
	/* �A�C�e���\�� */

	draw_manager.DrawCharacter(player); /* ��l���\�� */
	draw_manager.DrawCharacter(stair, width, height, static_cast<int>(player->GetPosX()), static_cast<int>(player->GetPosY()));  /* �K�i�\�� */

	/* �G�\�� */
	for (auto itr = enemy_list.begin(); itr != enemy_list.end(); itr++)
	{
		draw_manager.DrawCharacter((*itr), width, height, static_cast<int>((player)->GetPosX()), static_cast<int>((player)->GetPosY()));  /* �G�\�� */
	}
}
void GameMaster::DrawStatus()
{
	draw_manager.DrawStatusBar(player, floor_number);
}
void GameMaster::DrawAll()
{
	game_map->Update(); /* �}�b�v�X�V */
	CameraPos();  /* �J�����ݒ� */
	DrawMap();    /* �}�b�v�`�� */
	DrawStatus(); /* �X�e�[�^�X�`�� */
	draw_manager.DrawMiniMap(reinterpret_cast<MAPSET::DATA*>(game_map->GetMiniMap())); /* �~�j�}�b�v�`�� */
}
/* �e�[�u���Ǘ� */
/* �^�[�������̏����� */
void GameMaster::InitTurnProcessMap()
{
	manage_turn_process.emplace(GAME_STEP::INIT, &GameMaster::Init);
	manage_turn_process.emplace(GAME_STEP::CREATE_MAP, &GameMaster::CreateMap);
	manage_turn_process.emplace(GAME_STEP::DISPLAY_INFO, &GameMaster::DispInfo);
	manage_turn_process.emplace(GAME_STEP::TURN_START, &GameMaster::TurnStart);
	manage_turn_process.emplace(GAME_STEP::PLAYER_TURN, &GameMaster::PlayerTurn);
	manage_turn_process.emplace(GAME_STEP::STAIR_TURN, &GameMaster::StairTurn);
	manage_turn_process.emplace(GAME_STEP::ITEM_TURN, &GameMaster::ItemTurn);
	manage_turn_process.emplace(GAME_STEP::ENEMY_TURN, &GameMaster::EnemyTurn);
	manage_turn_process.emplace(GAME_STEP::STATUS_TURN, &GameMaster::StatusTurn);
	manage_turn_process.emplace(GAME_STEP::TURN_END, &GameMaster::TurnEnd);
	manage_turn_process.emplace(GAME_STEP::GAME_END, &GameMaster::GameEnd);
}