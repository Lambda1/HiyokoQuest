#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <iostream>

#include "..//RougeLikeMap/MapSet.hpp"
#include "./Direction.hpp"

/* キャラクタの座標型 */
using POS_TYPE = float;

/* 1ターン内の処理ステップ */
enum class TURN_MODE : int
{
	NONE,   /* 何もしていない */
	MOVE,   /* 移動中 */
	ATTACK, /* 攻撃中 */
	END,    /* 処理終了 */
};

class Character
{
	protected:
		/* 1ターン処理ステップ */
		TURN_MODE turn_cost;

		/* キャラ情報 */
		MAPSET::DATA chara_state;
		/* キャラ座標 */
		POS_TYPE x, y;
		/* キャラの向いている方向 */
		DIRECTION way;

		/* 消費系ステータス */
		int hp, mp;
		/* 標準ステータス */
		int max_hp, max_mp;
		int level, power, defence;
		/* 経験値 */
		int exp, next_level_exp;
		
		/* フラグ */
		bool death;     /* 死亡フラグ */
		bool is_friend; /* 友好フラグ */
		bool is_attack; /* 攻撃フラグ */

		/* 移動処理用変数 */
		POS_TYPE prev_x, prev_y; /* 前回の座標 */
		const float MOVE_RESOlUTION = 0.050f; /* 移動分解能 */
		const float M_EPSILON = 0.010f; /* 移動処理閾値 */
		const float ANG_DEG = 45.0f;

	public:
		Character();
		virtual ~Character();

		/* 初期化処理 */
		void InitPos(const POS_TYPE x, const POS_TYPE y);
		void InitStatus(const int level, const int power,const int defence);
		void InitMAX(const int max_hp,const int max_mp);
		
		/* 移動処理 */
		virtual void Move(DIRECTION direct) = 0;            /* 通常移動 */
		virtual void Teleport(const POS_TYPE x,const POS_TYPE y) = 0; /* テレポート移動 */
		
		/* ステータス処理 */
		void Damaged(const int damaged_value);       /* 受けたダメージ */
		int  Attack(const int damaged_side_defence); /* 与えるダメージ */
		void UseMP(const int used_mp);               /* 消費MP */
		void GetEXP(const int get_exp);              /* 獲得経験値 */
		
		/* 更新処理 */
		virtual void Update() = 0;

		/* ゲッタ */
		inline int  GetPower()   { return power; }
		inline int  GetDefence() { return defence; }
		inline POS_TYPE  GetPosX()    { return x; }
		inline POS_TYPE  GetPosY()    { return y; }
		inline POS_TYPE  GetPosPX() { return prev_x; }
		inline POS_TYPE  GetPosPY() { return prev_y; }
		inline bool GetFriend()  { return is_friend; }
		inline MAPSET::DATA GetCharaInfo() { return chara_state; }
		inline TURN_MODE GetTurnMode() { return turn_cost; }
		/* 方向を角度(度)に変換 */
		inline float GetAngle()
		{
			float ang = 0.0f;
			switch (way)
			{
			case DIRECTION::EAST:
				ang = ANG_DEG * 4.0f; break;
			case DIRECTION::WEST:
				ang = ANG_DEG * 0.0f; break;
			case DIRECTION::SOUTH:
				ang = ANG_DEG * 2.0f; break;
			case DIRECTION::NORTH:
				ang = ANG_DEG * 6.0f; break;
			case DIRECTION::SOUTH_EAST:
				ang = ANG_DEG * 3.0f; break;
			case DIRECTION::SOUTH_WEST:
				ang = ANG_DEG * 1.0f; break;
			case DIRECTION::NORTH_EAST:
				ang = ANG_DEG * 5.0f; break;
			case DIRECTION::NORTH_WEST:
				ang = ANG_DEG * 7.0f; break;
			default:
				break;
			}
			return ang;
		}

		/* セッタ */
		inline void SetTurnMode(const TURN_MODE mode) { turn_cost = mode; }

		/* 死亡判定 */
		inline bool IsDeath() { return death; }

		/* 演算子オーバーロード */
		inline bool operator==(const Character& rhs)
		{
			return (this->x == rhs.x) && (this->y == rhs.y);
		}
		inline bool operator!=(const Character& rhs)
		{
			return !(*this == rhs);
		}
};

#endif
