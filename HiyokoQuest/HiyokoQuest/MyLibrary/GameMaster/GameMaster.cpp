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

/* �Q�[���X�V���� */
void GameMaster::Update()
{
	/* �^�[������ */
	TurnProcess();

	/* �Q�[���`�� */
	if (game_step >= GAME_STEP::TURN_START)
	{
		DrawMap();
		DrawStatus();
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
/* ���������� */
void GameMaster::Init()
{
	std::cout << "GAME INIT" << std::endl;
	game_step = GAME_STEP::CREATE_MAP;

	/* �v���C���[���� */
	player = new Player;
	/* �G�l�~�[���� */

	/* �A�C�e������ */

}
/* �}�b�v�������� */
void GameMaster::CreateMap()
{
	std::cout << "GAME MAP" << std::endl;
	game_step = GAME_STEP::TURN_START;
	
	/* �K�w�ړ��̏ꍇ��, �}�b�v��j�����ė�����+1����. */
	if (game_map) {
		delete game_map;
		cnt_seed++;
	}
	/* �K�w��i�� */
	floor_number++;
	/* ������������ */
	room_number = floor_number;
	/* ��b���o�^ + �}�b�v���� */
	game_map = new RougeLikeMap<MAP_TYPE>(random_seed+cnt_seed);
	game_map->Init(width,height,room_number);
	game_map->SetBaseInfo(static_cast<MAP_TYPE>(MAPSET::DATA::NONE), static_cast<MAP_TYPE>(MAPSET::DATA::ROOM), static_cast<MAP_TYPE>(MAPSET::DATA::ROAD), static_cast<MAP_TYPE>(MAPSET::DATA::WALL));
	game_map->Generate();

	/* �v���C���[�z�u */
	int pos_x, pos_y;
	game_map->GetRoomPos(&pos_x, &pos_y); /* �����_���ɕ����̍��W���擾 */
	game_map->SetChara(pos_y, pos_x,static_cast<MAP_TYPE>(player->GetCharaInfo())); /* �}�b�v�Ƀv���C���[��z�u */
	player->InitPos(pos_x,pos_y); /* �v���C���[���W��������  */

	/* �G�l�~�[�z�u */

	/* �A�C�e���z�u */

	/* �e���C�������� */
	game_map->Update();

	MAP_TYPE* test = game_map->GetALL();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) std::cout << (int)test[i * width + j];
		std::cout << std::endl;
	}
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
	std::cout << "GAME PLAYER" << std::endl;

	/* �L�[���͂��������ꍇ, �������J�n */
	if (key_pos != BUTTON_MASK::NONE)
	{
		/* �v���C���[�ړ����� */
		if (!turn_cost_flag) {
			turn_cost_flag = PlayerMove();
		}
		/* �v���C���[�U������ */
		if (!turn_cost_flag) {
			turn_cost_flag = PlayerAttack();
		}
		/* �s��������Ă�����, �A�C�e���^�[���ֈڍs */
		if (turn_cost_flag)
		{
			player->Update(); /* �v���C���[���X�V */
			turn_cost_flag = false;
			game_step = GAME_STEP::ITEM_TURN;
		}
	}
}
/* �A�C�e���^�[������ */
void GameMaster::ItemTurn()
{
	std::cout << "GAME ITEM" << std::endl;
	game_step = GAME_STEP::TURN_START;
}

/* �L�����N�^�[���� */
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
	int pos_x = player->GetPosX(), pos_y = player->GetPosY();
	CalcDirectionToPos(&pos_x, &pos_y, static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR));
	/* �ړ��\�̏ꍇ, �v���C���[���ړ����� */
	if (IsPosMove(pos_x, pos_y)) {
		game_map->SetChara(player->GetPosY(), player->GetPosX(), static_cast<MAP_TYPE>(MAPSET::DATA::NONE)); /* ���ݒn�_���N���A */
		player->Move(static_cast<DIRECTION>(key_pos & BUTTON_MASK::CURSOR)); /* �ړ����� */
		game_map->SetChara(pos_y, pos_x, static_cast<MAP_TYPE>(player->GetCharaInfo())); /* �ړ��n�_��player��z�u */
		return true;
	}
	return false;
}
/* �v���C���[�U������ */
bool GameMaster::PlayerAttack()
{
	return false;
}

/* �`�揈�� */
void GameMaster::DrawMap()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GameMaster::DrawStatus()
{
}