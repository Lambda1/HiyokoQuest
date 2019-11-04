#ifndef __ENEMY_AI_HPP__
#define __ENEMY_AI_HPP__

namespace ENEMY_AI
{
	/* AI���[�h */
	enum class MODE : int
	{
		STANDARD, /* �W�����GAI */
		BERSERK,  /* �o�[�T�[�JAI */
		SIZE
	};
	/* ���E�͈� */
	enum class VISUAL_SIZE : int
	{
		AROUND = 3, /* ���̓}�X(3*3) */

		SMALL  = 6,
		NORMAL = 10,
		LARGE  = 20,
		LLARGE = 500,

		SIZE
	};
};

#endif
