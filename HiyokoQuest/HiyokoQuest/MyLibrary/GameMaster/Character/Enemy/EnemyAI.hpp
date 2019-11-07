#ifndef __ENEMY_AI_HPP__
#define __ENEMY_AI_HPP__

namespace ENEMY_AI
{
	/* AI���[�h */
	enum class MODE : int
	{
		STANDARD, /* �W�����GAI */
		BERSERK,  /* �o�[�T�[�JAI */
		A_STAR,   /* A-STAR�T�� */
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

	/* �}�b�v�Z�� */
	struct MapCell
	{
		enum class STATUS : int { NONE, OPEN, CLOSE };
		
		int actual_cost;
		int huristics_cost;
		STATUS status;
		int parent_x, parent_y;

		MapCell() : 
		actual_cost(0), huristics_cost(0), status(STATUS::NONE),
		parent_x(0), parent_y(0)
		{
		};
		MapCell(int act_cost, int hur_cost, int par_x, int par_y) :
			actual_cost(act_cost), huristics_cost(hur_cost), status(STATUS::OPEN),
			parent_x(par_x), parent_y(par_y)
		{
		};
		~MapCell()
		{
		};
		inline bool operator<(const MapCell& rhs) const
		{
			return (actual_cost > rhs.actual_cost);
		}
	};
};

#endif
