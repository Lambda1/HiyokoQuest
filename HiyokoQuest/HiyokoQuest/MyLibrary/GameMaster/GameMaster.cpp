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

/* �Q�[���X�V���� */
void GameMaster::Update()
{
	/* �^�[������ */
	TurnProcess();

	/* �Q�[���`�� */
	if (game_step >= GAME_STEP::TURN_START)
	{
		CameraPos();  /* �J�����ݒ� */
		DrawMap();    /* �}�b�v�`�� */
		DrawStatus(); /* �X�e�[�^�X�`�� */
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
/* ���������� */
void GameMaster::Init()
{
	std::cout << "GAME INIT" << std::endl;
	game_step = GAME_STEP::CREATE_MAP;

	/* �v���C���[���� */
	player = new Player;

	/* �`�揈�������� */
	draw_manager.Init();
}
/* �}�b�v�������� */
void GameMaster::CreateMap()
{
	std::cout << "GAME MAP" << std::endl;
	game_step = GAME_STEP::TURN_START;
	
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
	game_map->SetChara(stair_pos_y, stair_pos_x, static_cast<MAP_TYPE>(stair->GetCharaInfo())); /* �}�b�v�ɊK�i��z�u */
	stair->InitPos(static_cast<POS_TYPE>(stair_pos_x), static_cast<POS_TYPE>(stair_pos_y)); /* �K�i���W�������� */
	/* �G�l�~�[���� */

	/* �A�C�e������ */

	/* �v���C���[�z�u */
	int player_pos_x, player_pos_y;
	game_map->GetRoomPos(&player_pos_x, &player_pos_y); /* �����_���ɕ����̍��W���擾 */
	game_map->SetChara(player_pos_y, player_pos_x,static_cast<MAP_TYPE>(player->GetCharaInfo())); /* �}�b�v�Ƀv���C���[��z�u */
	player->InitPos(static_cast<POS_TYPE>(player_pos_x), static_cast<POS_TYPE>(player_pos_y)); /* �v���C���[���W��������  */

	/* �G�l�~�[�z�u */

	/* �A�C�e���z�u */

	/* �e���C�������� */
	game_map->Update();
}
/* �^�[���J�n���� */
void GameMaster::TurnStart()
{
	std::cout << "GAME START" << std::endl;
	game_step = GAME_STEP::PLAYER_TURN;
	/* �^�[�����o�� */
	turn_number++;
}
/* �v���C���[�^�[������ */
void GameMaster::PlayerTurn()
{
	//std::cout << "GAME PLAYER" << std::endl;

	/* �L�[���͂��������ꍇ, �������J�n */
	if (key_pos != BUTTON_MASK::NONE)
	{
		/* �s��������Ă��Ȃ��ꍇ�̏��� */
		if (player->GetTurnMode() == TURN_MODE::NONE)
		{
			/* �v���C���[�ړ����� (�\�����͗L��) */
			if (((key_pos & BUTTON_MASK::CURSOR) != BUTTON_MASK::NONE)) {
				/* �ړ��\�Ȃ��, �}�b�v���ړ����Ĉړ������Ɉڍs */
				if (PlayerMove()) { player->SetTurnMode(TURN_MODE::MOVE); }
			}
			/* �v���C���[�U������ (ABXY���͗L��) */
			if (((key_pos & BUTTON_MASK::ABXY) != BUTTON_MASK::NONE)) {
				if (PlayerAttack()) { player->SetTurnMode(TURN_MODE::ATTACK); }
			}
		}
	}
	/* �`��p�̏��� */
	if (player->GetTurnMode() == TURN_MODE::MOVE)
	{
		player->MoveAnimation();
	}
	/* �s��������Ă�����, ���^�[���ֈڍs */
	else if (player->GetTurnMode() == TURN_MODE::END)
	{
		player->Update(); /* �v���C���[���X�V */
		player->SetTurnMode(TURN_MODE::NONE);
		game_step = GAME_STEP::STAIR_TURN;
		
		game_map->Update();
	}
}
/* �K�w�^�[������ */
void GameMaster::StairTurn()
{
	std::cout << "GAME STAIR" << std::endl;
	
	/* Player��STAIR�̍��W����������, ���K�w�ֈړ� */
	if (*player == *stair)
	{
		DiposeFloor();
		game_step = GAME_STEP::CREATE_MAP;
	}
	else { game_step = GAME_STEP::ITEM_TURN; }
}
/* �A�C�e���^�[������ */
void GameMaster::ItemTurn()
{
	std::cout << "GAME ITEM" << std::endl;
	game_step = GAME_STEP::ENEMY_TURN;
}
/* �G�l�~�[�^�[������ */
void GameMaster::EnemyTurn()
{
	std::cout << "GAME ENEMY" << std::endl;
	game_step = GAME_STEP::STATUS_TURN;
}
/* �X�e�[�^�X�^�[������ */
void GameMaster::StatusTurn()
{
	std::cout << "GAME STATUS" << std::endl;
	game_step = GAME_STEP::TURN_END;
}
/* �^�[���I������ */
void GameMaster::TurnEnd()
{
	std::cout << "GAME END" << std::endl;
	game_step = GAME_STEP::TURN_START;
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
bool GameMaster::IsPosMove(const int x, const int y)
{
	MAPSET::DATA dungeon_data = static_cast<MAPSET::DATA>(game_map->GetLayerDungeon(x,y));

	/* �� and �����Ȃ��, �ړ��\ */
	if (dungeon_data == MAPSET::DATA::ROAD || dungeon_data == MAPSET::DATA::ROOM)
	{
		return true;
	}

	return false;
}

/* PlayerTurn��p���� */
/* �v���C���[�ړ����� */
bool GameMaster::PlayerMove()
{
	/* �v���C���[���i�s�����Ɉړ��\������ */
	POS_TYPE pos_x = player->GetPosX(), pos_y = player->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR));
	/* �ړ��\�̏ꍇ, �v���C���[���ړ����� */
	if (IsPosMove(static_cast<int>(pos_x), static_cast<int>(pos_y)))
	{
		game_map->SetChara(static_cast<int>(player->GetPosY()), static_cast<int>(player->GetPosX()), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* ���ݒn�_���N���A */
		player->Move(static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR)); /* �ړ����� */
		game_map->SetChara(static_cast<int>(pos_y), static_cast<int>(pos_x), static_cast<MAP_TYPE>(player->GetCharaInfo())); /* �ړ��n�_��player��z�u */
		return true;
	}
	return false;
}
/* �v���C���[�U������ */
bool GameMaster::PlayerAttack()
{
	return false;
}

/* StairTurn��p���� */
/* ����w�ɐi�ނ��߂̏���*/
/* �����X�V, ������������� */
void GameMaster::DiposeFloor()
{
	cnt_seed++;
	if (game_map) delete game_map;
	if (stair) delete stair;
}

/* �`�揈�� */
void GameMaster::CameraPos()
{
	draw_manager.CameraPos(player->GetPosPX(), 10.0f, player->GetPosPY() + 10.0f, player->GetPosPX(), 0.0f, player->GetPosPY());
}
void GameMaster::DrawMap()
{
	draw_manager.DrawInit(); /* ��ʃN���A */

	draw_manager.DrawMap(game_map->GetALL(), width, height, static_cast<int>(player->GetPosX()), static_cast<int>(player->GetPosY())); /* �}�b�v�\�� */
	/* �A�C�e���\�� */
	draw_manager.DrawCharacter(player); /* ��l���\�� */
}
void GameMaster::DrawStatus()
{
}