#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <iostream>

#include "..//RougeLikeMap/MapSet.hpp"
#include "./Direction.hpp"

class Character
{
	protected:
		/* �L������� */
		MAPSET::DATA chara_state;
		/* �L�������W */
		int x, y;
		
		/* ����n�X�e�[�^�X */
		int hp, mp;
		/* �W���X�e�[�^�X */
		int max_hp, max_mp;
		int level, power, defence;
		/* �o���l */
		int exp, next_level_exp;
		
		/* �t���O */
		bool death;     /* ���S�t���O */
		bool is_friend; /* �F�D�t���O */
	public:
		Character();
		virtual ~Character();

		/* ���������� */
		void InitPos(const int x, const int y);
		void InitStatus(const int level, const int power,const int defence);
		void InitMAX(const int max_hp,const int max_mp);
		
		/* �ړ����� */
		virtual void Move(DIRECTION direct) = 0;            /* �ʏ�ړ� */
		virtual void Teleport(const int x,const int y) = 0; /* �e���|�[�g�ړ� */
		
		/* �X�e�[�^�X���� */
		void Damaged(const int damaged_value);       /* �󂯂��_���[�W */
		int  Attack(const int damaged_side_defence); /* �^����_���[�W */
		void UseMP(const int used_mp);               /* ����MP */
		void GetEXP(const int get_exp);              /* �l���o���l */
		
		/* �X�V���� */
		virtual void Update() = 0;

		/* �Q�b�^ */
		inline int  GetPower()   { return power; }
		inline int  GetDefence() { return defence; }
		inline int  GetPosX()    { return x; }
		inline int  GetPosY()    { return y; }
		inline bool GetFriend()  { return is_friend; }
		inline MAPSET::DATA GetCharaInfo() { return chara_state; }

		/* ���S���� */
		inline bool IsDeath() { return death; }
};

#endif
