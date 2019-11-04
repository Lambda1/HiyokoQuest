#ifndef __ENEMY_AI_HPP__
#define __ENEMY_AI_HPP__

namespace ENEMY_AI
{
	/* AIモード */
	enum class MODE : int
	{
		STANDARD, /* 標準索敵AI */
		BERSERK,  /* バーサーカAI */
		SIZE
	};
	/* 視界範囲 */
	enum class VISUAL_SIZE : int
	{
		AROUND = 3, /* 周囲マス(3*3) */

		SMALL  = 6,
		NORMAL = 10,
		LARGE  = 20,
		LLARGE = 500,

		SIZE
	};
};

#endif
