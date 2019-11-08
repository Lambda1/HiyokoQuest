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

/* �L�����N�^�̋��ʏ��� */
namespace CommonCharacter
{
	inline static int CENSORING_COST = 1000; /* �ł��؂莎�s�� */

	/* dungeon����target�̍��W��Ԃ� */
	my_math::Vec<int> SearchTargetCoord(const MAP_TYPE* dungeon, const int& width, const int& height, const MAPSET::DATA& target);
	
	/* A-STAR�T�� */
	std::stack<my_math::Vec<int>> A_STAR(const MAP_TYPE *dungeon, const int &width,const int &height,const int &px,const int &py, const my_math::Vec<int> &target_pos);
	/* OPEN���X�g���̍ő剿�l��T�� */
	ENEMY_AI::MapCell* GetMaxValueItr(const std::list<ENEMY_AI::MapCell*>&node_list,ENEMY_AI::MapCell* now_p);
};

#endif