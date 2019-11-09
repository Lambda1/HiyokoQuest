#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <iostream>
#include <stack>

#include "..//..//Mathematics/MyMath/MyMath.hpp"

#include "..//RougeLikeMap/MapSet.hpp"
#include "./Direction.hpp"

#include "..//..//CommonProcessing/CommonCharacter.hpp"

#include "./Weapon/Weapon.hpp"

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

		/* 武器管理 */
		Weapon m_weapon;

		/* 消費系ステータス */
		int hp, mp;
		/* 標準ステータス */
		int max_hp, max_mp;
		int level, power, defence;
		/* 経験値 */
		int exp, next_level_exp;
		
		/* 敵用の識別ID */
		MAPSET::DATA enemy_type; /* 描画処理を統一させるため, Characterが管理する. */

		/* フラグ */
		bool death;     /* 死亡フラグ */
		bool is_friend; /* 友好フラグ */
		bool is_attack; /* 攻撃フラグ */
		MAPSET::DATA attacked_ch; /* 最後に攻撃されたキャラ */

		/* 移動処理用変数 */
		POS_TYPE prev_x, prev_y; /* 前回の座標 (アニメーションはこちらの座標を参照) */
		const float MOVE_RESOlUTION = 0.050f; /* 移動分解能 */
		const float M_EPSILON = 0.10f; /* 移動処理閾値 */
		const float ANG_DEG = 45.0f;

		void CalcMoveDirect(const POS_TYPE &val);

	public:
		Character();
		virtual ~Character();

		/* 初期化処理 */
		void InitPos(const POS_TYPE x, const POS_TYPE y);
		void InitStatus(const int level, const int power,const int defence);
		void InitMAX(const int max_hp,const int max_mp);
		
		/* 移動処理 */
		virtual void Move(DIRECTION direct) = 0; /* 通常移動 */
		virtual void Teleport(const POS_TYPE x,const POS_TYPE y) = 0; /* テレポート移動 */
		
		/* ステータス処理 */
		void Damaged(const int damaged_value);       /* 受けたダメージ */
		int  Attack(const int damaged_side_defence); /* 与えるダメージ */
		void UseMP(const int used_mp);               /* 消費MP */
		void GetEXP(const int get_exp);              /* 獲得経験値 */
		virtual int GiveEXP()=0;                     /* 与える経験値 */

		/* 更新処理 */
		virtual void Update() = 0;

		/* アニメーション */
		void MoveAnimation();
		void AttackAnimation();

		/* ゲッタ */
		inline int  GetPower()   { return power; }
		inline int  GetDefence() { return defence; }
		inline int  GetHP() { return hp; }
		inline int  GetMaxHP() { return max_hp; }
		inline int  GetLevel() { return level; }
		inline POS_TYPE  GetPosX()  const  { return x; }
		inline POS_TYPE  GetPosY()  const  { return y; }
		inline POS_TYPE  GetPosPX() const { return prev_x; }
		inline POS_TYPE  GetPosPY() const { return prev_y; }
		inline bool GetFriend()  { return is_friend; }
		inline MAPSET::DATA GetCharaInfo() { return chara_state; }
		inline TURN_MODE GetTurnMode() { return turn_cost; }
		inline DIRECTION GetDirect() { return way; }
		inline MAPSET::DATA GetAttacked() { return attacked_ch; }
		inline MAPSET::DATA GetEnemyID() { return enemy_type; }
		inline int GetTransEnemyID() { return (static_cast<int>(enemy_type)-static_cast<int>(MAPSET::DATA::ENEMY1)); }  /* バイアス変換した識別ID */
		/* 方向を角度(度)に変換 */
		inline POS_TYPE GetAngle() { return CommonCharacter::TransAngle(way); }

		/* セッタ */
		inline void SetDirection(const DIRECTION direct) { way = direct; }
		inline void SetTurnMode(const TURN_MODE mode) { turn_cost = mode; }
		inline void SetAttacked(const MAPSET::DATA data) { attacked_ch = data; }

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
