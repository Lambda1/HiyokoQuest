#ifndef __ENEMY_AI_HPP__
#define __ENEMY_AI_HPP__

namespace ENEMY_AI
{
	/* AI[h */
	enum class MODE : int
	{
		STANDARD, /* WõGAI */
		BERSERK,  /* o[T[JAI */
		SIZE
	};
	/* EÍÍ */
	enum class VISUAL_SIZE : int
	{
		SMALL  = 6,
		NORMAL = 20,
		LARGE  = 30,

		SIZE
	};
};

#endif
