#ifndef __ENEMY_GENERATOR_HPP__
#define __ENEMY_GENERATOR_HPP__

#include "../../RougeLikeMap/MapSet.hpp"
#include "./EnemyAI.hpp"

namespace ENEMY_GENERATOR
{
	struct EnemyDef
	{
		MAPSET::DATA enemy_type;
		double probablity;
		int strong_rate;
		ENEMY_AI::MODE mode;
		
		EnemyDef():
		enemy_type(MAPSET::DATA::ENEMY1),
		probablity(0), strong_rate(0),
		mode(ENEMY_AI::MODE::STANDARD)
		{}
		EnemyDef(const MAPSET::DATA &type,const double &prob,const int &rate,const ENEMY_AI::MODE &ai_mode) :
		enemy_type(type),
		probablity(prob), strong_rate(rate),
		mode(ai_mode)
		{}
		~EnemyDef(){}
	};

	inline static const EnemyDef generate_manager[] =
	{
		/* ENEMYÇÃéÌóﬁ, î≠ê∂ämó¶(70,15,5,8,2)%, Ç¬ÇÊÇ≥, AIÇÃéÌóﬁ */
		{MAPSET::DATA::ENEMY1, 0.70,  1, ENEMY_AI::MODE::STANDARD},
		{MAPSET::DATA::ENEMY2, 0.85,  2, ENEMY_AI::MODE::STANDARD},
		{MAPSET::DATA::ENEMY3, 0.90,  4, ENEMY_AI::MODE::BERSERK},
		{MAPSET::DATA::ENEMY4, 0.98,  7, ENEMY_AI::MODE::BERSERK},
		{MAPSET::DATA::ENEMY5, 1.00, 10, ENEMY_AI::MODE::A_STAR},
	};

	inline static constexpr int size = sizeof(generate_manager) / sizeof(generate_manager[0]);
};

#endif
