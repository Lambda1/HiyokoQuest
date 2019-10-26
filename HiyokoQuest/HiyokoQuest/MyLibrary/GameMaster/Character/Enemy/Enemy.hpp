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
		const int first_next_exp = first_level;

		/* �X�V���� */
		void JudgeDeath();
	
	public:
		Enemy();
		Enemy(const float& up_rate); /* �p���[�A�b�v�� */
		~Enemy();

		void Move(DIRECTION direct) override;
		void Teleport(const POS_TYPE x, const POS_TYPE y) override;

		void Update() override;

		/* �Q�b�^ */
		inline int GiveEXP() override { return next_level_exp; } /* �^����EXP */
};

#endif
