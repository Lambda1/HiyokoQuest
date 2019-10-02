#ifndef __GAME_KEY_HPP__
#define __GAME_KEY_HPP__

/* �Q�[�����̃L�[�z�u */

enum class BUTTON : int
{
	UP = static_cast<int>('w'),
	DOWN = static_cast<int>('s'),
	RIGHT = static_cast<int>('d'),
	LEFT = static_cast<int>('a'),
	OK = static_cast<int>('x'),
	CANCEL = static_cast<int>('z'),
	NONE = 0
};

/* ���4bit: ABXY�{�^�� */
/* ����4bit: �\���{�^�� */
enum class BUTTON_MASK : unsigned char
{
	/* �\���{�^�� */
	/* 4���� */
	UP    = 0x01,
	DOWN  = 0x02,
	RIGHT = 0x04,
	LEFT  = 0x08,
	/* �΂ߕ��� */
	UP_RIGHT    = UP | RIGHT,
	UP_LEFT     = UP | LEFT,
	DOWN_RIGHT  = DOWN | RIGHT,
	DOWN_LEFT   = DOWN | LEFT,
	
	/* ABXY�{�^�� */
	/* ����/�L�����Z��/�U��/���j���[ */
	OK     = 0x10,
	CANCEL = 0x20,
	ATTACK = 0x40,
	MENU   = 0x80,

	/* �}�X�Nbit */
	CURSOR = 0x0F,
	ABXY   = 0xF0,

	/* �����L�[���� */
	UP_DOWN    = UP | DOWN,
	RIGHT_LEFT = RIGHT | LEFT,
	OK_CANCEL  = OK | CANCEL,

	/* ���͂Ȃ� */
	NONE = 0x00
};
/* BUTTON_MASK: bit���Z�̒�` */
constexpr BUTTON_MASK operator|(BUTTON_MASK L,BUTTON_MASK R)
{
	return static_cast<BUTTON_MASK>(static_cast<unsigned char>(L) | static_cast<unsigned char>(R));
}
constexpr BUTTON_MASK operator&(BUTTON_MASK L, BUTTON_MASK R)
{
	return static_cast<BUTTON_MASK>(static_cast<unsigned char>(L) & static_cast<unsigned char>(R));
}

constexpr int ALPHABET = 26;
constexpr char KEY_BIAS = 'a';

constexpr int MAX_KEY_INPUT = 2;

#endif