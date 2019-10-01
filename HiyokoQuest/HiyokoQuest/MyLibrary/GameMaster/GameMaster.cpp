#include "./GameMaster.hpp"

GameMaster::GameMaster() :
	game_step(GAME_STEP::INIT),
	game_over_flag(false),
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
/* �Q�[������ */
void GameMaster::Init()
{
	std::cout << "GAME INIT" << std::endl;
	game_step = GAME_STEP::CREATE_MAP;

	/* �v���C���[���� */
	player = new Player;
	/* �G�l�~�[���� */

	/* �A�C�e������ */

}
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
void GameMaster::TurnStart()
{
	std::cout << "GAME START" << std::endl;
	game_step = GAME_STEP::PLAYER_TURN;
	/* �^�[�����o�� */
	turn_number++;
}
void GameMaster::PlayerTurn()
{
	std::cout << "GAME PLAYER" << std::endl;
	game_step = GAME_STEP::ITEM_TURN;

	/* �v���C���[���X�V */
	player->Update();
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