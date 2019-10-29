#ifndef __WEAPON_HPP__
#define __WEAPON_HPP__

#include <stack>

#include "./WeaponDefine.hpp"
#include "..//Direction.hpp"

class Weapon
{
	/* 現在の武器 */
	WeaponDef m_weapon; /* 武器の種類 */

	/* 武器座標 */
	POS_TYPE x, y;
	/* キャラ方向 */
	DIRECTION way;
	
	/* 移動処理用変数 */
	POS_TYPE prev_x, prev_y; /* 前回の座標 */
	const float MOVE_RESOlUTION = 0.050f; /* 移動分解能 */
	const float M_EPSILON = 0.10f; /* 移動処理閾値 */
	const float ANG_DEG = 45.0f;

	/* アニメーション処理用変数 */
	int frame_counter; /* フレームカウンタ */
	const float attack_move = 0.3f; /* 攻撃アニメーションの移動量 */
	std::stack<POS_TYPE> pos_stack;

	private:
		/* アニメーション */
		bool Fist();  /* 素手 */
		void Sword(); /* 剣 */

		void CalcMoveDirect(const POS_TYPE& val);
		void MoveAnimation();
	public:
		Weapon();
		~Weapon();

		/* 武器によるアニメーション */
		bool Animation();

		/* ゲッタ */
		inline WeaponDef GetWepon() { return m_weapon; }
		inline void GetCharaPos(POS_TYPE &x, POS_TYPE &y, POS_TYPE &prev_x,POS_TYPE &prev_y)
		{
			x = this->x, y = this->y, prev_x = this->prev_x, prev_y = this->prev_y;
		}

		/* セッタ */
		inline void SetWeapon(const WeaponDef weapon) { m_weapon = weapon; }
		inline void SetCharaInfo(const POS_TYPE& x, const POS_TYPE& y, const DIRECTION& way) { this->x = x, this->y = y, this->way = way; }
};

#endif
