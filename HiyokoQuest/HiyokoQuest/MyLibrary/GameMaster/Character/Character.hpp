#ifndef __CHARACTER_HPP__
#define __CHARACTER_HPP__

#include <iostream>
#include <stack>

#include "..//RougeLikeMap/MapSet.hpp"
#include "./Direction.hpp"

#include "./Weapon/Weapon.hpp"

/* 1�^�[�����̏����X�e�b�v */
enum class TURN_MODE : int
{
	NONE,   /* �������Ă��Ȃ� */
	MOVE,   /* �ړ��� */
	ATTACK, /* �U���� */
	END,    /* �����I�� */
};

class Character
{
	protected:
		/* 1�^�[�������X�e�b�v */
		TURN_MODE turn_cost;

		/* �L������� */
		MAPSET::DATA chara_state;
		/* �L�������W */
		POS_TYPE x, y;
		/* �L�����̌����Ă������ */
		DIRECTION way;

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
		bool is_attack; /* �U���t���O */
		MAPSET::DATA attacked_ch; /* �Ō�ɍU�����ꂽ�L���� */

		/* �ړ������p�ϐ� */
		POS_TYPE prev_x, prev_y; /* �O��̍��W */
		const float MOVE_RESOlUTION = 0.050f; /* �ړ�����\ */
		const float M_EPSILON = 0.10f; /* �ړ�����臒l */
		const float ANG_DEG = 45.0f;

		Weapon m_weapon;

		void CalcMoveDirect(const POS_TYPE &val);
	public:
		Character();
		virtual ~Character();

		/* ���������� */
		void InitPos(const POS_TYPE x, const POS_TYPE y);
		void InitStatus(const int level, const int power,const int defence);
		void InitMAX(const int max_hp,const int max_mp);
		
		/* �ړ����� */
		virtual void Move(DIRECTION direct) = 0;            /* �ʏ�ړ� */
		virtual void Teleport(const POS_TYPE x,const POS_TYPE y) = 0; /* �e���|�[�g�ړ� */
		
		/* �X�e�[�^�X���� */
		void Damaged(const int damaged_value);       /* �󂯂��_���[�W */
		int  Attack(const int damaged_side_defence); /* �^����_���[�W */
		void UseMP(const int used_mp);               /* ����MP */
		void GetEXP(const int get_exp);              /* �l���o���l */
		virtual int GiveEXP()=0;                     /* �^����o���l */

		/* �X�V���� */
		virtual void Update() = 0;

		/* �A�j���[�V���� */
		void MoveAnimation();
		void AttackAnimation();

		/* �Q�b�^ */
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
		/* �������p�x(�x)�ɕϊ� */
		inline float GetAngle()
		{
			float ang = 0.0f;
			switch (way)
			{
			case DIRECTION::SOUTH:
				ang = ANG_DEG * 0.0f; break;
			case DIRECTION::SOUTH_EAST:
				ang = ANG_DEG * 1.0f; break;
			case DIRECTION::EAST:
				ang = ANG_DEG * 2.0f; break;
			case DIRECTION::NORTH_EAST:
				ang = ANG_DEG * 3.0f; break;
			case DIRECTION::NORTH:
				ang = ANG_DEG * 4.0f; break;
			case DIRECTION::NORTH_WEST:
				ang = ANG_DEG * 5.0f; break;
			case DIRECTION::WEST:
				ang = ANG_DEG * 6.0f; break;
			case DIRECTION::SOUTH_WEST:
				ang = ANG_DEG * 7.0f; break;
			default:
				break;
			}
			return ang;
		}

		/* �Z�b�^ */
		inline void SetDirection(const DIRECTION direct) { way = direct; }
		inline void SetTurnMode(const TURN_MODE mode) { turn_cost = mode; }
		inline void SetAttacked(const MAPSET::DATA data) { attacked_ch = data; }

		/* ���S���� */
		inline bool IsDeath() { return death; }

		/* ���Z�q�I�[�o�[���[�h */
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
