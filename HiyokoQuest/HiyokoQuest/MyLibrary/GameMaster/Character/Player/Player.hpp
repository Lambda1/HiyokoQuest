#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "../Character.hpp"
#include "..//Weapon/Weapon.hpp"

class Player : public Character
{
	private:
		/* 初期ステータス */
		const int first_hp = 10, first_mp = 10;
		const int first_level = 1;
		const int first_power = 5, first_defence = 1;
		const int first_next_exp = first_level * 2;

		/* 更新処理 */
		void LevelUp();
		void JudgeDeath();
	
	public:
		Player();
		~Player();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;

		int GiveEXP() override { return exp; }
};

#endif
