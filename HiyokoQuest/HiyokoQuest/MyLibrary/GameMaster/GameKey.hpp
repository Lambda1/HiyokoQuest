#ifndef __GAME_KEY_HPP__
#define __GAME_KEY_HPP__

/* ゲーム中のキー配置 */

enum class BUTTON : int
{
	UP = static_cast<int>('w'),
	DOWN = static_cast<int>('x'),
	RIGHT = static_cast<int>('d'),
	LEFT = static_cast<int>('a'),
	RIGHT_UP = static_cast<int>('e'),
	LEFT_UP = static_cast<int>('q'),
	RIGHT_DOWN = static_cast<int>('c'),
	LEFT_DOWN = static_cast<int>('z'),
	OK = static_cast<int>('s'),
	CANCEL = static_cast<int>('v'),
	NONE = 0
};

#endif
