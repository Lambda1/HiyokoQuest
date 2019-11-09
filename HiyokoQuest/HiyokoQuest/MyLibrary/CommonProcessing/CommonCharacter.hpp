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
	/* ヒューリスティックコストの計算 */
	inline static double CalcCost_A_STAR(const my_math::Vec<int>& target_pos, const int& px, const int& py)
	{
		return my_math::Math::Distance<double>(target_pos.x,target_pos.y,px,py);

		//int tmp_x = abs(target_pos.x - px);
		//int tmp_y = abs(target_pos.y - py);
		//return tmp_x > tmp_y ? tmp_x : tmp_y;
	}
	/* 貪欲法で最小ヒューリスティックコストを探索 */
	inline static ENEMY_AI::MapCell* SearchMinHuristicCost(const std::list<ENEMY_AI::MapCell*> &node_list)
	{
		ENEMY_AI::MapCell* attract_p = (*node_list.rbegin());
		for (auto itr = node_list.rbegin(); (*itr)->parent != nullptr; ++itr)
		{
			if (attract_p->huristics_cost > (*itr)->huristics_cost) attract_p = *itr;
		}
		return attract_p;
	}
public:
	CommonCharacter& operator=(const CommonCharacter& rhs) = delete;

	/* 座標間の方向を取得 */
	/* NOTE: 不正な座標時は,NONEを返す. */
	template<class T>
	static DIRECTION GetVector(const T& x, const T& y, const T& px, const T& py)
	{
		if (x == px && y == py) return DIRECTION::NONE;
		if (x < 0 || y < 0 || px < 0 || py < 0) return DIRECTION::NONE;
		POS_TYPE ang = static_cast<POS_TYPE>(my_math::Math::Angle<T>(x, y, px, py) * 180.0f / my_math::Math::PI);
		for (int i = 0; i < DIRECT_TABLE::table_size; i++)
		{
			if (DIRECT_TABLE::manager_direct[i].m_ang1 <= ang && ang <= DIRECT_TABLE::manager_direct[i].m_ang2)
			{
				return DIRECT_TABLE::manager_direct[i].m_direct;
			}
		}
		return DIRECTION::NONE;
	}

	/* 方向を角度に変換 */
	static POS_TYPE TransAngle(const DIRECTION &way);

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
	/* 周囲のtarget位置を探索 */
	/* NOTE: PLAYERの自動化にも使用するため, 共通処理 */
	inline static DIRECTION isNeighborTarget(const MAP_TYPE* dungeon, const int& width, const int& height, my_math::Vec<int> pos, const MAPSET::DATA& target_id)
	{
		for (int i = -static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; i <= static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; ++i)
		{
			if ((pos.y + i) < 0 || (pos.y + i) > height - 1) continue;
			for (int j = -static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; j <= static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; ++j)
			{
				if ((pos.x + j) < 0 || (pos.x + j) > width - 1) continue;
				if (dungeon[(pos.y + i) * width + (pos.x+j)] == static_cast<MAP_TYPE>(target_id)) return GetVector<int>(pos.x, pos.y, pos.x + j, pos.y + i);
			}
		}
		return DIRECTION::NONE;
	}
};

#endif