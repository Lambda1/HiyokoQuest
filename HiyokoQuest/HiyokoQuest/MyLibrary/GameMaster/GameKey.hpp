#ifndef __GAME_KEY_HPP__
#define __GAME_KEY_HPP__

/* ゲーム中のキー配置 */

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

/* 上位4bit: ABXYボタン */
/* 下位4bit: 十字ボタン */
enum class BUTTON_MASK : unsigned char
{
	/* 十字ボタン */
	/* 4方向 */
	UP    = 0x01,
	DOWN  = 0x02,
	RIGHT = 0x04,
	LEFT  = 0x08,
	/* 斜め方向 */
	UP_RIGHT    = UP | RIGHT,
	UP_LEFT     = UP | LEFT,
	DOWN_RIGHT  = DOWN | RIGHT,
	DOWN_LEFT   = DOWN | LEFT,
	
	/* ABXYボタン */
	/* 決定/キャンセル/攻撃/メニュー */
	OK     = 0x10,
	CANCEL = 0x20,
	ATTACK = 0x40,
	MENU   = 0x80,

	/* マスクbit */
	CURSOR = 0x0F,
	ABXY   = 0xF0,

	/* 無効キー入力 */
	UP_DOWN    = UP | DOWN,
	RIGHT_LEFT = RIGHT | LEFT,
	OK_CANCEL  = OK | CANCEL,

	/* 入力なし */
	NONE = 0x00
};
/* BUTTON_MASK: bit演算の定義 */
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