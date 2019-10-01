#ifndef __GAME_MASTER_HPP__
#define __GAME_MASTER_HPP__

#include <GL/freeglut.h>

#include <iostream>
#include <ctime>

#include "./RougeLikeMap/RougeLikeMap.hpp"
#include "./RougeLikeMap/MapSet.hpp"
#include "./GameKey.hpp"

#include "./Character/Player/Player.hpp"

class GameMaster
{
	/* �Q�[���̏����X�e�b�v */
	enum class GAME_STEP : int
	{
		INIT,        /* ������: ���߂�1��̂� */
		CREATE_MAP,  /* �}�b�v���� */
		TURN_START,  /* �^�[���J�n */
		PLAYER_TURN, /* �v���C���[�t�F�[�Y */
		ITEM_TURN,   /* �A�C�e���t�F�[�Y */
		ENEMY_TURN,  /* �G�l�~�[�t�F�[�Y */
		STATUS_TURN, /* �X�e�[�^�X�t�F�[�Y */
		TURN_END,    /* �^�[���I�� */
	};

	/* �Q�[������ */
	GAME_STEP game_step; /* �Q�[���i�s�Ǘ� */
	bool game_over_flag; /* �Q�[���I�[�o�Ǘ� */

	/* �������� */
	int random_seed, cnt_seed;     /* �����V�[�h */

	/* �}�b�v���� */
	using MAP_TYPE = unsigned char; /* �}�b�v�^��ߖ� */
	RougeLikeMap<MAP_TYPE>* game_map; /* �Q�[���}�b�v */
	int floor_number, turn_number, room_number; /* �K�w, �o�߃^�[��, ������ */
	const int width = 120, height = 100; /* �}�b�v�̍L��(�Œ�) */

	/* �v���C���[���� */
	Character* player;

	/* �G�l�~�[���� */

	/* �A�C�e������ */

private:
	/* �^�[������ */
	void Init();       /* ������ */
	void CreateMap();  /* �}�b�v���� */
	void TurnStart();  /* �^�[���J�n */
	void PlayerTurn(); /* �v���C���[�^�[�� */
	void ItemTurn();   /* �A�C�e���^�[�� */
	void EnemyTurn();  /* �G�l�~�[�^�[�� */
	void StatusTurn(); /* �X�e�[�^�X�^�[�� */
	void TurnEnd();    /* �^�[���I�� */

	/* �`�揈�� */
	void DrawMap();    /* �}�b�v�`�� */
	void DrawStatus(); /* �X�e�[�^�X�`�� */

public:
		GameMaster();
		~GameMaster();

		/* �Q�[���X�V */
		void Update();

		/* �L�[���͒���� */
		void KeyInput(bool *key_on, bool *key_off);

		/* �Q�b�^ */
		inline bool IsGameOver() { return game_over_flag; }
};

#endif
