#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <iostream>

#include "..//RougeLikeMap/MapSet.hpp"
#include "./Direction.hpp"

class Character
{
	protected:
		/* キャラ情報 */
		MAPSET::DATA chara_state;
		/* キャラ座標 */
		int x, y;
		
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
	public:
		Character();
		virtual ~Character();

		/* 初期化処理 */
		void InitPos(const int x, const int y);
		void InitStatus(const int level, const int power,const int defence);
		void InitMAX(const int max_hp,const int max_mp);
		
		/* 移動処理 */
		virtual void Move(DIRECTION direct) = 0;            /* 通常移動 */
		virtual void Teleport(const int x,const int y) = 0; /* テレポート移動 */
		
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
		inline int  GetPosX()    { return x; }
		inline int  GetPosY()    { return y; }
		inline bool GetFriend()  { return is_friend; }
		inline MAPSET::DATA GetCharaInfo() { return chara_state; }

		/* 死亡判定 */
		inline bool IsDeath() { return death; }
};

#endif
