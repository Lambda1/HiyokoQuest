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
	inline static int CalcCost_A_STAR(const my_math::Vec<int>& target_pos, const int& px, const int& py)
	{
		int tmp_x = (target_pos.x - px);
		int tmp_y = (target_pos.y - py);
		return tmp_x > tmp_y ? tmp_x : tmp_y;
	}
public:
	CommonCharacter& operator=(const CommonCharacter& rhs) = delete;

	/* ���W�Ԃ̕������擾 */
	static DIRECTION GetVector(const POS_TYPE &x,const POS_TYPE &y,const POS_TYPE& px, const POS_TYPE& py);

	/* ���ݒn������������̃}�b�v�����擾 */
	static MAPSET::DATA ToDirectData(POS_TYPE px, POS_TYPE py, const MAP_TYPE* dungeon, const DIRECTION& direct, const int& width);

	/* ���E�����̃C���f�b�N�X�擾 */
	static void VisualRarnge(int* x, int* y, int* index_x, int* index_y, const int& weight, const DIRECTION& way);

	/* dungeon����target�̍��W��Ԃ� */
	static my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon, const int& width, const int& height, const MAPSET::DATA& target);
	
	/* A-STAR�T��(���R�X�g) */
	/* NOTE: PLAYER�̎������ɂ��g�p���邽��,���ʏ����Ƃ���. */
	static std::stack<my_math::Vec<int>> A_STAR(const MAP_TYPE *dungeon, const int &width,const int &height,const int &px,const int &py, const my_math::Vec<int> &target_pos);
};

#endif