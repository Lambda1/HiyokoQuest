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
		SMALL  = 4,
		NORMAL = 6,
		LARGE  = 11,

		SIZE
	};
};

#endif
