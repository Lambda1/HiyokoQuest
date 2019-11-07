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
	/* 初期ステータス */
	const int first_hp = 5, first_mp = 0;
	const int first_level = 1;
	const int first_power = 2, first_defence = 1;
	const int first_next_exp = first_level;

	/* AI処理 */
	ENEMY_AI::MODE ai_mode;

	/* Aスター用 */
	bool need_research_route; /* ルート再検索フラグ */
	std::stack<my_math::Vec<int>> route_pos;
	std::function<int(const int &,const int &)> calc_cost;

	/* 視界範囲 */
	ENEMY_AI::VISUAL_SIZE visual_field;
	
	/* 標的位置 */
	my_math::Vec<int> target_pos;

	/* 更新処理 */
	void JudgeDeath();
		
	/* AI処理 */
	DIRECTION Standard(const MAP_TYPE* dungeon, const int& width, const int& height);
	DIRECTION Berserk(const MAP_TYPE* dungeon, const int& width, const int& height);
	DIRECTION A_STAR(const MAP_TYPE* dungeon, const int& width, const int& height);

	public:
		Enemy();
		Enemy(const float& up_rate,const MAPSET::DATA &id); /* パワーアップ版 */
		~Enemy();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;

		/* Enemy専用 */
		DIRECTION AI_Mode(const MAP_TYPE *dungeon,const int &width,const int &height);

		/* ゲッタ */
		inline int GiveEXP() override { return next_level_exp; } /* 与えるEXP */

		/* セッタ */
		inline void SetAI(const ENEMY_AI::MODE mode) { ai_mode = mode; }
		inline void SetTargetPos(const int x, const int y) { target_pos.Set(x,y); }
};

#endif
