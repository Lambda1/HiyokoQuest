#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "../Character.hpp"

class Enemy : public Character
{
	private:
		/* �����X�e�[�^�X */
		const int first_hp = 5, first_mp = 0;
		const int first_level = 1;
		const int first_power = 2, first_defence = 1;
		const int first_next_exp = 0;

		/* �X�V���� */
		void JudgeDeath();
	
	public:
		Enemy();
		~Enemy();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;
};

#endif
