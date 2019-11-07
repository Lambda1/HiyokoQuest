#ifndef __ENEMY_AI_HPP__
#define __ENEMY_AI_HPP__

namespace ENEMY_AI
{
	/* AIモード */
	enum class MODE : int
	{
		STANDARD, /* 標準索敵AI */
		BERSERK,  /* バーサーカAI */
		A_STAR,   /* A-STAR探索 */
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

	/* マップセル */
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
