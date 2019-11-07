#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "../Character.hpp"
#include "./EnemyAI.hpp"

#include "..//..//RougeLikeMap/MapSet.hpp"
#include "..//Direction.hpp"

#include "..//..//..//Mathematics/MyVector/MyVector.hpp"

#include <functional>
#include <list>
#include <stack>

class Enemy : public Character
{
private:
	/* �����X�e�[�^�X */
	const int first_hp = 5, first_mp = 0;
	const int first_level = 1;
	const int first_power = 2, first_defence = 1;
	const int first_next_exp = first_level;

	/* AI���� */
	ENEMY_AI::MODE ai_mode;

	/* A�X�^�[�p */
	bool need_research_route; /* ���[�g�Č����t���O */
	std::stack<my_math::Vec<int>> route_pos;
	std::function<int(const int &,const int &)> calc_cost;

	/* ���E�͈� */
	ENEMY_AI::VISUAL_SIZE visual_field;
	
	/* �W�I�ʒu */
	my_math::Vec<int> target_pos;

	/* �X�V���� */
	void JudgeDeath();
		
	/* AI���� */
	DIRECTION Standard(const MAP_TYPE* dungeon, const int& width, const int& height);
	DIRECTION Berserk(const MAP_TYPE* dungeon, const int& width, const int& height);
	DIRECTION A_STAR(const MAP_TYPE* dungeon, const int& width, const int& height);

	public:
		Enemy();
		Enemy(const float& up_rate,const MAPSET::DATA &id); /* �p���[�A�b�v�� */
		~Enemy();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;

		/* Enemy��p */
		DIRECTION AI_Mode(const MAP_TYPE *dungeon,const int &width,const int &height);

		/* �Q�b�^ */
		inline int GiveEXP() override { return next_level_exp; } /* �^����EXP */

		/* �Z�b�^ */
		inline void SetAI(const ENEMY_AI::MODE mode) { ai_mode = mode; }
		inline void SetTargetPos(const int x, const int y) { target_pos.Set(x,y); }
};

#endif
