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
	/* 初期ステータス */
	const int first_hp = 5, first_mp = 0;
	const int first_level = 1;
	const int first_power = 2, first_defence = 1;
	const int first_next_exp = first_level;

	/* AI処理 */
	ENEMY_AI::MODE ai_mode;
	bool need_research_route; /* ルート再検索フラグ */

	/* Aスター用 */
	std::stack<my_math::Vec<int>> route_pos;

	/* 視界範囲 */
	ENEMY_AI::VISUAL_SIZE visual_field;
	
	/* 標的データ */
	MAPSET::DATA target_id;

	/* 更新処理 */
	void JudgeDeath();
		
	/* AIモード */
	/* 標準索敵 */
	/* NOTE: 発見時->追跡, 未発見時->ランダム移動 */
	DIRECTION Standard(const MAP_TYPE *dungeon, const int& width, const int& height);
	DIRECTION A_STAR  (const MAP_TYPE *dungeon, const int& width, const int& height);

	/* 攻撃判定 */
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

		/* Enemy専用 */
		DIRECTION AI_Mode(const MAP_TYPE *dungeon,const int &width,const int &height);

		/* ゲッタ */
		inline int GiveEXP() override { return next_level_exp; } /* 与えるEXP */

		/* セッタ */
		inline void SetAI(const ENEMY_AI::MODE mode) { ai_mode = mode; }
		inline void SetSearchRouteFlag(const bool& flag) { need_research_route = flag; }
		inline void SetTargetID(const MAPSET::DATA& target_id) { this->target_id = target_id; }
};

#endif
