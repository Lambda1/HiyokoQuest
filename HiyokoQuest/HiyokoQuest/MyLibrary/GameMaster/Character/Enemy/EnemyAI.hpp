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
		
		int x, y;
		int actual_cost;
		double huristics_cost;
		STATUS status;
		MapCell* parent;

		MapCell() : 
		x(0), y(0),
		actual_cost(0), huristics_cost(0), status(STATUS::NONE),
		parent(nullptr)
		{
		};
		MapCell(const int &px, const int &py, const int& act_cost, const double& hur_cost, MapCell *cell) :
			x(px), y(py),
			actual_cost(act_cost), huristics_cost(hur_cost), status(STATUS::OPEN),
			parent(cell)
		{
		};
		~MapCell()
		{
		};
		inline double GetScoreASTAR()
		{
			return actual_cost + huristics_cost;
		}
	};
};

#endif
