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
		/* 初期ステータス */
		const int first_hp = 5, first_mp = 0;
		const int first_level = 1;
		const int first_power = 2, first_defence = 1;
		const int first_next_exp = first_level;

		/* AIモード */
		ENEMY_AI::MODE ai_mode;

		/* 視界範囲 */
		ENEMY_AI::VISUAL_SIZE visual_field;

		/* テーブル管理 */
		std::map<ENEMY_AI::MODE, DIRECTION(Enemy::*)(const MAP_TYPE*,const int &,const int &)> manage_ai_table;

		/* テーブル生成 */
		/* AIテーブル初期化 */
		/* NOTE: 敵ごとにテーブルを生成するため, inline化 */
		inline void InitTableAI()
		{
			manage_ai_table.emplace(ENEMY_AI::MODE::STANDARD, &Enemy::Standard);
			manage_ai_table.emplace(ENEMY_AI::MODE::BERSERK,  &Enemy::Berserk);
		}

		/* 更新処理 */
		void JudgeDeath();
	
		/* AI処理 */
		DIRECTION Standard(const MAP_TYPE* dungeon, const int& width, const int& height);
		DIRECTION Berserk(const MAP_TYPE* dungeon, const int& width, const int& height);

	public:
		Enemy();
		Enemy(const float& up_rate,const MAPSET::DATA &id); /* パワーアップ版 */
		~Enemy();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;

		/* Enemy専用 */
		DIRECTION AI_Move(const MAP_TYPE *dungeon,const int &width,const int &height);

		/* ゲッタ */
		inline int GiveEXP() override { return next_level_exp; } /* 与えるEXP */

		/* セッタ */
		inline void SetAI(const ENEMY_AI::MODE mode) { ai_mode = mode; }
};

#endif
