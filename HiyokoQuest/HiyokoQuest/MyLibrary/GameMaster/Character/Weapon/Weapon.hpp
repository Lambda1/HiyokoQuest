#ifndef __WEAPON_HPP__
#define __WEAPON_HPP__

#include <iostream>
#include <stack>

#include "./WeaponDefine.hpp"
#include "..//Direction.hpp"

/*
 Characterクラスと同様の処理があるが, 意味が異なるので注意.
 素手: 移動アニメーションと素手攻撃アニメーション (アニメーションを流用)
*/

class Weapon
{
	/* 現在の武器 */
	WeaponDef m_weapon; /* 武器の種類 */

	/* 武器座標・方向 */
	POS_TYPE *x, *y;
	DIRECTION *way;
	POS_TYPE *prev_x, *prev_y;

	/* 移動処理用変数 */
	const float MOVE_RESOlUTION = 0.050f; /* 移動分解能 */
	const float M_EPSILON = 0.10f; /* 移動処理閾値 */
	const float ANG_DEG = 45.0f;

	/* アニメーション処理用変数 */
	int frame_counter; /* フレームカウンタ */
	const float attack_move = 0.3f; /* 攻撃アニメーションの移動量 */
	std::stack<POS_TYPE> pos_stack;

	private:
		/* アニメーション */
		bool Fist();  /* 素手処理 */
		void BodyBlowAnimation(); /* 素手: アニメーション */
		bool Sword(); /* 剣処理 */

		/* 移動量制御 */
		void CalcMoveDirect(const POS_TYPE& val);
	public:
		Weapon();
		~Weapon();

		/* 武器によるアニメーション */
		bool Animation();

		/* ゲッタ */
		inline WeaponDef GetWepon() { return m_weapon; }

		/* セッタ */
		inline void SetWeapon(const WeaponDef weapon) { m_weapon = weapon; }
		inline void SetCharaInfoPtr(POS_TYPE *x, POS_TYPE *y, DIRECTION *way) { this->x = x, this->y = y, this->way = way; }
		inline void SetCharaPrevPtr(POS_TYPE* prev_x, POS_TYPE* prev_y) { this->prev_x = prev_x, this->prev_y = prev_y; }
};

#endif
