#ifndef __WEAPON_HPP__
#define __WEAPON_HPP__

#include <stack>

#include "./WeaponDefine.hpp"
#include "..//Direction.hpp"

class Weapon
{
	/* ���݂̕��� */
	WeaponDef m_weapon; /* ����̎�� */

	/* ������W */
	POS_TYPE x, y;
	/* �L�������� */
	DIRECTION way;
	
	/* �ړ������p�ϐ� */
	POS_TYPE prev_x, prev_y; /* �O��̍��W */
	const float MOVE_RESOlUTION = 0.050f; /* �ړ�����\ */
	const float M_EPSILON = 0.10f; /* �ړ�����臒l */
	const float ANG_DEG = 45.0f;

	/* �A�j���[�V���������p�ϐ� */
	int frame_counter; /* �t���[���J�E���^ */
	const float attack_move = 0.3f; /* �U���A�j���[�V�����̈ړ��� */
	std::stack<POS_TYPE> pos_stack;

	private:
		/* �A�j���[�V���� */
		bool Fist();  /* �f�� */
		void Sword(); /* �� */

		void CalcMoveDirect(const POS_TYPE& val);
		void MoveAnimation();
	public:
		Weapon();
		~Weapon();

		/* ����ɂ��A�j���[�V���� */
		bool Animation();

		/* �Q�b�^ */
		inline WeaponDef GetWepon() { return m_weapon; }
		inline void GetCharaPos(POS_TYPE &x, POS_TYPE &y, POS_TYPE &prev_x,POS_TYPE &prev_y)
		{
			x = this->x, y = this->y, prev_x = this->prev_x, prev_y = this->prev_y;
		}

		/* �Z�b�^ */
		inline void SetWeapon(const WeaponDef weapon) { m_weapon = weapon; }
		inline void SetCharaInfo(const POS_TYPE& x, const POS_TYPE& y, const DIRECTION& way) { this->x = x, this->y = y, this->way = way; }
};

#endif
