#ifndef __DIRECTION_HPP__
#define __DIRECTION_HPP__

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

#endif
