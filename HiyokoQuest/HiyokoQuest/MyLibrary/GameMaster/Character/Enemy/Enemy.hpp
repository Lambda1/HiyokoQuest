#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "../Character.hpp"
#include "./EnemyAI.hpp"

#include "..//..//RougeLikeMap/MapSet.hpp"
#include "..//Direction.hpp"

#include <map>

class Enemy : public Character
{
	private:
		/* �����X�e�[�^�X */
		const int first_hp = 5, first_mp = 0;
		const int first_level = 1;
		const int first_power = 2, first_defence = 1;
		const int first_next_exp = first_level;

		/* AI���[�h */
		ENEMY_AI::MODE ai_mode;

		/* ���E�͈� */
		ENEMY_AI::VISUAL_SIZE visual_field;

		/* �e�[�u���Ǘ� */
		std::map<ENEMY_AI::MODE, DIRECTION(Enemy::*)(const MAP_TYPE*,const int &,const int &)> manage_ai_table;

		/* �e�[�u������ */
		/* AI�e�[�u�������� */
		/* NOTE: �G���ƂɃe�[�u���𐶐����邽��, inline�� */
		inline void InitTableAI()
		{
			manage_ai_table.emplace(ENEMY_AI::MODE::STANDARD, &Enemy::Standard);
			manage_ai_table.emplace(ENEMY_AI::MODE::BERSERK,  &Enemy::Berserk);
		}

		/* �X�V���� */
		void JudgeDeath();
	
		/* AI���� */
		DIRECTION Standard(const MAP_TYPE* dungeon, const int& width, const int& height);
		DIRECTION Berserk(const MAP_TYPE* dungeon, const int& width, const int& height);

	public:
		Enemy();
		Enemy(const float& up_rate,const MAPSET::DATA &id); /* �p���[�A�b�v�� */
		~Enemy();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;

		/* Enemy��p */
		DIRECTION AI_Move(const MAP_TYPE *dungeon,const int &width,const int &height);

		/* �Q�b�^ */
		inline int GiveEXP() override { return next_level_exp; } /* �^����EXP */

		/* �Z�b�^ */
		inline void SetAI(const ENEMY_AI::MODE mode) { ai_mode = mode; }
};

#endif
