#ifndef __GAME_MASTER_HPP__
#define __GAME_MASTER_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <ctime>
#include <list>

#include "./RougeLikeMap/RougeLikeMap.hpp"
#include "./RougeLikeMap/MapSet.hpp"
#include "./GameKey.hpp"

#include "./Character/Player/Player.hpp"
#include "./Character/Stair/Stair.hpp"
#include "./Character/Enemy/Enemy.hpp"

#include "..//DrawGame/DrawGame.hpp"

class GameMaster
{
	/* �Q�[���̏����X�e�b�v */
	enum class GAME_STEP : int
	{
		INIT,        /* ������: ���߂�1��̂� */
		CREATE_MAP,  /* �}�b�v���� */
		TURN_START,  /* �^�[���J�n */
		PLAYER_TURN, /* �v���C���[�t�F�[�Y */
		STAIR_TURN,  /* �K�i�t�F�[�Y */
		ITEM_TURN,   /* �A�C�e���t�F�[�Y */
		ENEMY_TURN,  /* �G�l�~�[�t�F�[�Y */
		STATUS_TURN, /* �X�e�[�^�X�t�F�[�Y */
		TURN_END,    /* �^�[���I�� */
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

	/* �}�b�v���� */
	using MAP_TYPE = unsigned char; /* �}�b�v�^��ߖ� */
	RougeLikeMap<MAP_TYPE>* game_map; /* �Q�[���}�b�v */
	int floor_number, turn_number, room_number; /* �K�w, �o�߃^�[��, ������ */
	const int width = 70, height = 50; /* �}�b�v�̍L��(�Œ�) */

	/* �v���C���[���� */
	Player* player;

	/* �K�i���� */
	Stair* stair;

	/* �G�l�~�[���� */
	std::list<Character*> enemy_list;

	/* �A�C�e������ */

	/* �`�揈�� */
	DrawGame draw_manager; /* �`��Ǘ� */

private:
	/* �^�[������ */
	void TurnProcess(); /* �^�[���������� */
	void Init();       /* ������ */
	void CreateMap();  /* �}�b�v���� */
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

	/* PlayerTurn��p���� */
	bool PlayerMove();   /* �v���C���[�ړ����� */
	bool PlayerAttack(); /* �v���C���[�U������ */

	/* StairTuen��p���� */
	void DiposeFloor();
	void DiposeEnemy();

	/* �`�揈�� */
	void CameraPos();
	void DrawMap();    /* �}�b�v�`�� */
	void DrawStatus(); /* �X�e�[�^�X�`�� */

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
