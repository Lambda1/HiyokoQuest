#ifndef __DIRECTION_HPP__
#define __DIRECTION_HPP__

#include <map>

/* �L�����N�^�[�̈ړ����� */
/* GameKey.hpp�ƑΉ������� */

/* �L�����N�^�̍��W�^ */
using POS_TYPE = float;

enum class DIRECTION : unsigned char
{
	EAST  = 0x04, /* �E */
	WEST  = 0x08, /* �� */
	SOUTH = 0x02, /* �� */
	NORTH = 0x01, /* �� */
	
	SOUTH_EAST = EAST | SOUTH,  /* �E�� */
	SOUTH_WEST = WEST | SOUTH,  /* ���� */
	NORTH_EAST = EAST | NORTH,  /* �E�� */
	NORTH_WEST = WEST | NORTH,  /* �E�� */

	NONE = 0x00 /* �ړ��Ȃ� */
};

/* �e�[�u���p�f�[�^�\�� */
namespace DIRECT_TABLE
{
	class DIRECTION_TABLE
	{
	private:
		DIRECTION_TABLE() :
			m_ang1(0), m_ang2(0), m_direct(DIRECTION::NONE)
		{};
	public:
		POS_TYPE m_ang1, m_ang2;
		DIRECTION m_direct;

		DIRECTION_TABLE(const POS_TYPE &ang1,const POS_TYPE &ang2,const DIRECTION &direct) :
			m_ang1(ang1), m_ang2(ang2), m_direct(direct)
		{};
		~DIRECTION_TABLE() {};
	};

	/* �p�x����\ */
	inline static constexpr POS_TYPE ANG_DEG  = 45.0f;
	
	/* �p�x�e�[�u�� */
	inline static const DIRECTION_TABLE manager_direct[] =
	{
		/* [-45, 45) */
		{-ANG_DEG * 1.0f,  ANG_DEG * 1.0f, DIRECTION::EAST},
		
		/* [45, 135) */
		{ ANG_DEG * 1.0f,  ANG_DEG * 3.0f, DIRECTION::SOUTH},

		/* [-135, -45) */
		{-ANG_DEG * 3.0f, -ANG_DEG * 1.0f, DIRECTION::NORTH},

		/* [-180, -135) && [135, 180) */
		{-ANG_DEG * 4.0f, -ANG_DEG * 3.0f, DIRECTION::WEST},
		{ ANG_DEG * 3.0f,  ANG_DEG * 4.0f, DIRECTION::WEST}
	};

	/* �e�[�u���T�C�Y */
	inline static constexpr int table_size = sizeof(manager_direct) / sizeof(manager_direct[0]);
};
#endif