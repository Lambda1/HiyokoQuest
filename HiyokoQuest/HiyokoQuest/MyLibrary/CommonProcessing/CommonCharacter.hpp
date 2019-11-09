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
#include "..//GameMaster/Character/Direction.hpp"

/* キャラクタの共通処理 */
/* NOTE: 共通処理のため,staticクラスとする. */
class CommonCharacter
{
	inline static constexpr int CENSORING_COST = 10000; /* 打ち切り試行回数 */
	inline static constexpr int visual_range = static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND); /* 視界範囲 */

	CommonCharacter();
	~CommonCharacter();

	/* A-STAR探索 */
	/* OPENリスト中の最大価値を探索 */
	static ENEMY_AI::MapCell* GetMaxValueItr(const std::list<ENEMY_AI::MapCell*>& node_list, ENEMY_AI::MapCell* now_p);
	inline static int CalcCost_A_STAR(const my_math::Vec<int>& target_pos, const int& px, const int& py)
	{
		int tmp_x = (target_pos.x - px);
		int tmp_y = (target_pos.y - py);
		return tmp_x > tmp_y ? tmp_x : tmp_y;
	}
public:
	CommonCharacter& operator=(const CommonCharacter& rhs) = delete;

	/* 座標間の方向を取得 */
	static DIRECTION GetVector(const POS_TYPE &x,const POS_TYPE &y,const POS_TYPE& px, const POS_TYPE& py);

	/* 現在地から向き方向のマップ情報を取得 */
	static MAPSET::DATA ToDirectData(POS_TYPE px, POS_TYPE py, const MAP_TYPE* dungeon, const DIRECTION& direct, const int& width);

	/* 視界方向のインデックス取得 */
	static void VisualRarnge(int* x, int* y, int* index_x, int* index_y, const int& weight, const DIRECTION& way);

	/* dungeon内のtargetの座標を返す */
	static my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon, const int& width, const int& height, const MAPSET::DATA& target);
	
	/* A-STAR探索(高コスト) */
	/* NOTE: PLAYERの自動化にも使用するため,共通処理とした. */
	static std::stack<my_math::Vec<int>> A_STAR(const MAP_TYPE *dungeon, const int &width,const int &height,const int &px,const int &py, const my_math::Vec<int> &target_pos);
};

#endif