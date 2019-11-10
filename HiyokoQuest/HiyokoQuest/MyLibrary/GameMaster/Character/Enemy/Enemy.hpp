#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "../Character.hpp"
#include "./EnemyAI.hpp"

#include "..//..//RougeLikeMap/MapSet.hpp"
#include "..//Direction.hpp"

#include "..//..//..//Mathematics/MyVector/MyVector.hpp"

#include "..//..//..//CommonProcessing/CommonCharacter.hpp"

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
	bool need_research_route; /* ���[�g�Č����t���O */

	/* A�X�^�[�p */
	std::stack<my_math::Vec<int>> route_pos;

	/* ���E�͈� */
	ENEMY_AI::VISUAL_SIZE visual_field;
	
	/* �W�I�f�[�^ */
	MAPSET::DATA target_id;

	/* �X�V���� */
	void JudgeDeath();
		
	/* AI���[�h */
	/* �W�����G */
	/* NOTE: ������->�ǐ�, ��������->�����_���ړ� */
	DIRECTION Standard(const MAP_TYPE *dungeon, const int& width, const int& height);
	DIRECTION A_STAR  (const MAP_TYPE *dungeon, const int& width, const int& height);

	/* �U������ */
	inline bool isTargetToDirect(const MAP_TYPE *dungeon, const int& width, const DIRECTION& candidate)
	{
		return (CommonCharacter::ToDirectData(x, y, dungeon, candidate, width) == target_id);
	}

	public:
		Enemy();
		Enemy(const float& up_rate,const MAPSET::DATA &id,const ENEMY_AI::MODE &mode);
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
		inline void SetSearchRouteFlag(const bool& flag) { need_research_route = flag; }
		inline void SetTargetID(const MAPSET::DATA& target_id) { this->target_id = target_id; }
};

#endif
