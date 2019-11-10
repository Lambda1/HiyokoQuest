#ifndef __GAME_MASTER_HPP__
#define __GAME_MASTER_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <ctime>
#include <list>
#include <map>
#include <functional>
#include <random>

#include "./RougeLikeMap/RougeLikeMap.hpp"
#include "./RougeLikeMap/MapSet.hpp"
#include "./GameKey.hpp"

#include "./Character/Player/Player.hpp"
#include "./Character/Stair/Stair.hpp"
#include "./Character/Enemy/Enemy.hpp"

#include "./Character/Enemy/EnemyGenerator.hpp"

#include "..//DrawGame/DrawGame.hpp"

class GameMaster
{
	/* �Q�[���̏����X�e�b�v */
	enum class GAME_STEP : int
	{
		INIT,        /* ������: ���߂�1��̂� */
		CREATE_MAP,  /* �}�b�v���� */
		DISPLAY_INFO,/* �_���W�������\�� */
		TURN_START,  /* �^�[���J�n */
		PLAYER_TURN, /* �v���C���[�t�F�[�Y */
		ITEM_TURN,   /* �A�C�e���t�F�[�Y */
		ENEMY_TURN,  /* �G�l�~�[�t�F�[�Y */
		STATUS_TURN, /* �X�e�[�^�X�t�F�[�Y */
		TURN_END,    /* �^�[���I�� */
		STAIR_TURN,  /* �K�i�t�F�[�Y */
		GAME_END,    /* �Q�[���I�� */
	};

	/* �Q�[������ */
	GAME_STEP game_step; /* �Q�[���i�s�Ǘ� */
	bool game_over_flag; /* �Q�[���I�[�o�Ǘ� */
	
	/* �L�[���� */
	BUTTON_MASK key_pos;   /* �L�[1�������̃o�b�t�@ */
	bool all_key_release;  /* �S�L�[���͊Ď� */

	/* �������� */
	int random_seed, cnt_seed;     /* �����V�[�h */
	std::mt19937 mt_rnd;

	/* �}�b�v���� */
	RougeLikeMap<MAP_TYPE>* game_map; /* �Q�[���}�b�v */
	int floor_number, turn_number, room_number; /* �K�w, �o�߃^�[��, ������ */
	const int width = 70, height = 50; /* �}�b�v�̍L��(�Œ�) */

	/* �v���C���[���� */
	Player* player;

	/* �K�i���� */
	Stair* stair;

	/* �G�l�~�[���� */
	std::list<Enemy*> enemy_list;

	/* �A�C�e������ */

	/* �`�揈�� */
	DrawGame draw_manager; /* �`��Ǘ� */
	const float BASE_FPS = 60.0f; /* ��b�t���[��(1�b) */

	/* �e�[�u���Ǘ� */
	std::map<GAME_STEP, void(GameMaster::*)()> manage_turn_process; /* �^�[������ */

private:
	/* �������� */
	void InitDungeon();
	void InitEnemy();
	Enemy* EnemyGenerate();

	/* �^�[������ */
	void TurnProcess(); /* �^�[���������� */
	void Init();       /* ������ */
	void CreateMap();  /* �}�b�v���� */
	void DispInfo();   /* �_���W�������\�� */
	void TurnStart();  /* �^�[���J�n */
	void PlayerTurn(); /* �v���C���[�^�[�� */
	void StairTurn();  /* �K�w�^�[�� */
	void ItemTurn();   /* �A�C�e���^�[�� */
	void EnemyTurn();  /* �G�l�~�[�^�[�� */
	void StatusTurn(); /* �X�e�[�^�X�^�[�� */
	void TurnEnd();    /* �^�[���I�� */
	void GameEnd();    /* �Q�[���I�� */

	/* �L�����N�^�[���� */
	void CalcDirectionToPos(POS_TYPE *x,POS_TYPE *y,DIRECTION direct); /* �i�s�����̍��W���擾 */
	bool IsPosMove(const int x,const int y); /* ���W(x,y)�n�_��, �i�s�\������ */
	MAPSET::DATA IsPosAttack(const int& x, const int& y);
	bool CharacterMove(Character *ch_data, const DIRECTION &direct); /* �ėp�ړ����� */
	bool CharacterAttack(Character *ch_data); /* �ėp�U������ */
	bool AnimationUpdate(); /* �A�j���[�V�������� */

	/* StairTurn��p���� */
	void DiposeFloor();
	void DiposeEnemy();

	/* EnemyTurn��p���� */
	/* iterator�͔j��΍�̂���, ������ɍX�V����� */
	bool EnemyDeath(std::list<Enemy*>::iterator &enemy_itr);

	/* �`�揈�� */
	void CameraPos();
	void DrawMap();    /* �}�b�v�`�� */
	void DrawStatus(); /* �X�e�[�^�X�`�� */
	void DrawAll();

	/* �e�[�u���Ǘ� */
	void InitTurnProcessMap();

public:
		GameMaster();
		~GameMaster();

		/* �Q�[���X�V */
		void Update();

		/* �L�[���͒���� */
		void KeyInput(const bool *key_on,const bool *key_off);

		/* �Q�b�^ */
		inline bool IsGameOver() { return game_over_flag; }
};

#endif
