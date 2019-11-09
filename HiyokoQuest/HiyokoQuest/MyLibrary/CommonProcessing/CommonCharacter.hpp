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

/* �L�����N�^�̋��ʏ��� */
/* NOTE: ���ʏ����̂���,static�N���X�Ƃ���. */
class CommonCharacter
{
	inline static constexpr int CENSORING_COST = 10000; /* �ł��؂莎�s�� */
	inline static constexpr int visual_range = static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND); /* ���E�͈� */

	CommonCharacter();
	~CommonCharacter();

	/* A-STAR�T�� */
	/* OPEN���X�g���̍ő剿�l��T�� */
	static ENEMY_AI::MapCell* GetMaxValueItr(const std::list<ENEMY_AI::MapCell*>& node_list, ENEMY_AI::MapCell* now_p);
	/* �q���[���X�e�B�b�N�R�X�g�̌v�Z */
	inline static double CalcCost_A_STAR(const my_math::Vec<int>& target_pos, const int& px, const int& py)
	{
		return my_math::Math::Distance<double>(target_pos.x,target_pos.y,px,py);

		//int tmp_x = abs(target_pos.x - px);
		//int tmp_y = abs(target_pos.y - py);
		//return tmp_x > tmp_y ? tmp_x : tmp_y;
	}
	/* �×~�@�ōŏ��q���[���X�e�B�b�N�R�X�g��T�� */
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

	/* ���W�Ԃ̕������擾 */
	/* NOTE: �s���ȍ��W����,NONE��Ԃ�. */
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

	/* �������p�x�ɕϊ� */
	static POS_TYPE TransAngle(const DIRECTION &way);

	/* ���ݒn������������̃}�b�v�����擾 */
	static MAPSET::DATA ToDirectData(POS_TYPE px, POS_TYPE py, const MAP_TYPE* dungeon, const DIRECTION& direct, const int& width);

	/* ���E�����̃C���f�b�N�X�擾 */
	static void VisualRarnge(int* x, int* y, int* index_x, int* index_y, const int& weight, const DIRECTION& way);

	/* dungeon����target�̍��W��T�� */
	/* NOTE: ���݂��Ȃ��ꍇ, (-1,-1,0)��Ԃ�. */
	static my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon, const int& width, const int& height, const MAPSET::DATA& target);
	/* �͈͓���target�̍��W��T�� */
	static my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon,const my_math::Vec<int> &start, const my_math::Vec<int> &end,const my_math::Vec<int> &ch_pos,const int &width,const int &height, const MAPSET::DATA& target);

	/* A-STAR�T��(���R�X�g) */
	/* NOTE: PLAYER�̎������ɂ��g�p���邽��,���ʏ����Ƃ���. */
	static std::stack<my_math::Vec<int>> A_STAR(const MAP_TYPE *dungeon, const int &width,const int &height,const int &px,const int &py, const my_math::Vec<int> &target_pos);
	/* ���͂�target�ʒu��T�� */
	/* NOTE: PLAYER�̎������ɂ��g�p���邽��, ���ʏ��� */
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