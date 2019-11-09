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
	/* NOTE: 不正な座標時は,NONEを返す. */
	template<class T>
	static DIRECTION GetVector(const T& x, const T& y, const T& px, const T& py)
	{
		if (x < 0 || y < 0 || px < 0 || py < 0) return DIRECTION::NONE;
		int ang = static_cast<int>(my_math::Math::Angle<T>(x, y, px, py) * 180.0 / my_math::Math::PI);
		for (int i = 0; i < DIRECT_TABLE::table_size; i++)
		{
			if (DIRECT_TABLE::manager_direct[i].m_ang1 <= ang && ang <= DIRECT_TABLE::manager_direct[i].m_ang2)
			{
				return DIRECT_TABLE::manager_direct[i].m_direct;
			}
		}
		return DIRECTION::NONE;
	}

	/* 現在地から向き方向のマップ情報を取得 */
	static MAPSET::DATA ToDirectData(POS_TYPE px, POS_TYPE py, const MAP_TYPE* dungeon, const DIRECTION& direct, const int& width);

	/* 視界方向のインデックス取得 */
	static void VisualRarnge(int* x, int* y, int* index_x, int* index_y, const int& weight, const DIRECTION& way);

	/* dungeon内のtargetの座標を探索 */
	/* NOTE: 存在しない場合, (-1,-1,0)を返す. */
	static my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon, const int& width, const int& height, const MAPSET::DATA& target);
	/* 範囲内のtargetの座標を探索 */
	static my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon,const my_math::Vec<int> &start, const my_math::Vec<int> &end,const my_math::Vec<int> &ch_pos,const int &width,const int &height, const MAPSET::DATA& target);

	/* A-STAR探索(高コスト) */
	/* NOTE: PLAYERの自動化にも使用するため,共通処理とした. */
	static std::stack<my_math::Vec<int>> A_STAR(const MAP_TYPE *dungeon, const int &width,const int &height,const int &px,const int &py, const my_math::Vec<int> &target_pos);
};

#endif