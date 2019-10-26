#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "../Character.hpp"

class Enemy : public Character
{
	private:
		/* 初期ステータス */
		const int first_hp = 5, first_mp = 0;
		const int first_level = 1;
		const int first_power = 2, first_defence = 1;
		const int first_next_exp = first_level;

		/* 更新処理 */
		void JudgeDeath();
	
	public:
		Enemy();
		Enemy(const float& up_rate); /* パワーアップ版 */
		~Enemy();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;

		/* ゲッタ */
		inline int GiveEXP() override { return next_level_exp; } /* 与えるEXP */
};

#endif
