#ifndef __WEAPON_HPP__
#define __WEAPON_HPP__

#include <iostream>
#include <stack>

#include "./WeaponDefine.hpp"
#include "..//Direction.hpp"

/*
 Character�N���X�Ɠ��l�̏��������邪, �Ӗ����قȂ�̂Œ���.
 �f��: �ړ��A�j���[�V�����Ƒf��U���A�j���[�V���� (�A�j���[�V�����𗬗p)
*/

class Weapon
{
	/* ���݂̕��� */
	WeaponDef m_weapon; /* ����̎�� */

	/* ������W�E���� */
	POS_TYPE *x, *y;
	DIRECTION *way;
	POS_TYPE *prev_x, *prev_y;

	/* �ړ������p�ϐ� */
	const float MOVE_RESOlUTION = 0.050f; /* �ړ�����\ */
	const float M_EPSILON = 0.10f; /* �ړ�����臒l */
	const float ANG_DEG = 45.0f;

	/* �A�j���[�V���������p�ϐ� */
	int frame_counter; /* �t���[���J�E���^ */
	const float attack_move = 0.3f; /* �U���A�j���[�V�����̈ړ��� */
	std::stack<POS_TYPE> pos_stack;

	private:
		/* �A�j���[�V���� */
		bool Fist();  /* �f�菈�� */
		void BodyBlowAnimation(); /* �f��: �A�j���[�V���� */
		bool Sword(); /* ������ */

		/* �ړ��ʐ��� */
		void CalcMoveDirect(const POS_TYPE& val);
	public:
		Weapon();
		~Weapon();

		/* ����ɂ��A�j���[�V���� */
		bool Animation();

		/* �Q�b�^ */
		inline WeaponDef GetWepon() { return m_weapon; }

		/* �Z�b�^ */
		inline void SetWeapon(const WeaponDef weapon) { m_weapon = weapon; }
		inline void SetCharaInfoPtr(POS_TYPE *x, POS_TYPE *y, DIRECTION *way) { this->x = x, this->y = y, this->way = way; }
		inline void SetCharaPrevPtr(POS_TYPE* prev_x, POS_TYPE* prev_y) { this->prev_x = prev_x, this->prev_y = prev_y; }
};

#endif
