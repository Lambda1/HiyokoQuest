#ifndef __COMMON_CHARACTER_HPP__
#define __COMMON_CHARACTER_HPP__

#include <iostream>
#include <list>
#include <stack>
#include <vector>

#include "..//Mathematics/MyMath/MyMath.hpp"
#include "..//Mathematics/MyVector/MyVector.hpp"

#include "..//GameMaster///Character/Enemy/EnemyAI.hpp"
#include "..//GameMaster/RougeLikeMap/MapSet.hpp"

/* キャラクタの共通処理 */
namespace CommonCharacter
{
	inline static int CENSORING_COST = 1000; /* 打ち切り試行回数 */

	/* dungeon内のtargetの座標を返す */
	my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon, const int& width, const int& height, const MAPSET::DATA& target);
	
	/* A-STAR探索 */
	std::stack<my_math::Vec<int>> A_STAR(const MAP_TYPE *dungeon, const int &width,const int &height,const int &px,const int &py, const my_math::Vec<int> &target_pos);
	/* OPENリスト中の最大価値を探索 */
	ENEMY_AI::MapCell* GetMaxValueItr(const std::list<ENEMY_AI::MapCell*>&node_list,ENEMY_AI::MapCell* now_p);
};

#endif