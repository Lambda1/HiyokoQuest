#include "./Enemy.hpp"

Enemy::Enemy() :
	ai_mode(ENEMY_AI::MODE::STANDARD), visual_field(ENEMY_AI::VISUAL_SIZE::NORMAL),
	need_research_route(true), target_id(MAPSET::DATA::PLAYER)
{
	chara_state = MAPSET::DATA::ENEMY;

	hp = max_hp = first_hp;
	mp = max_mp = first_mp;
	level = first_level;
	power = first_power;
	defence = first_defence;
	next_level_exp = first_next_exp;
	
	enemy_type = MAPSET::DATA::ENEMY1;

	is_friend = false;
}

Enemy::Enemy(const float &up_rate,const MAPSET::DATA &id) :
	ai_mode(ENEMY_AI::MODE::BERSERK), visual_field(ENEMY_AI::VISUAL_SIZE::SMALL),
	need_research_route(true), target_id(MAPSET::DATA::PLAYER)
{
	chara_state = MAPSET::DATA::ENEMY;
	enemy_type = id;

	hp = max_hp = static_cast<int>(first_hp+up_rate);
	mp = max_mp = static_cast<int>(first_mp+up_rate);
	level = first_level;
	power = static_cast<int>(first_power+up_rate);
	defence = static_cast<int>(first_defence+up_rate);
	next_level_exp = static_cast<int>(first_level + first_next_exp*up_rate);

	is_friend = false;
}

Enemy::~Enemy()
{

}
/* override */
void Enemy::Move(DIRECTION direct)
{
	/* 描画のために, 前回の座標を保存 */
	prev_x = x, prev_y = y;
	way = direct;
	/* 移動座標の計算 */
	CalcMoveDirect(1.0f);
}
void Enemy::Teleport(const POS_TYPE x, const POS_TYPE y)
{
	prev_x = x, prev_y = y;
	this->x = x;
	this->y = y;
}
void Enemy::Update()
{
	JudgeDeath();
}
/* AI処理 */
DIRECTION Enemy::AI_Mode(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	/* EnemyAIは, 効率化のためテーブル化しない */
	switch (ai_mode)
	{
	case ENEMY_AI::MODE::STANDARD:
		way = Standard(dungeon, width, height); break;
	case ENEMY_AI::MODE::BERSERK:
		way = Berserk(dungeon, width, height); break;
	case ENEMY_AI::MODE::A_STAR:
		way = A_STAR(dungeon, width, height); break;
	default:
		break;
	}
	return way;
}
/* private */

/* 更新処理 */
/* 死亡判定 */
void Enemy::JudgeDeath()
{
	if (hp <= 0) {
		death = true;
	}
}

/* AI処理 */
/* 標準索敵AI */
DIRECTION Enemy::Standard(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;
	return DIRECTION::NONE;
}
/* バーサーカAI */
DIRECTION Enemy::Berserk(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;
	
	DIRECTION candidate = DIRECTION::NONE;
	int px = static_cast<int>(x), py = static_cast<int>(y);
	int nx = 0, ny = 0;
	int index_x = 0,index_y = 0;
	POS_TYPE dest_x = static_cast<POS_TYPE>(x), dest_y = static_cast<POS_TYPE>(y);
	
	/* 視界内の敵を探索 */
	VisualRarnge(&nx, &ny, &index_x, &index_y, static_cast<int>(visual_field));
	for (int i = ny; i < index_y; i++)
	{
		if ((py + i) < 0 || (py + i) > height - 1) continue;
		for (int j = nx; j < index_x; j++)
		{
			if ((px + j) < 0 || (px + j) > width - 1) continue;
			if (dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::PLAYER))
			{
				dest_x = static_cast<POS_TYPE>(px + j), dest_y = static_cast<POS_TYPE>(py + i);
				candidate = GetVector(dest_x, dest_y);
				break;
			}
		}
		if (candidate != DIRECTION::NONE) break;
	}
	/* 視界内に敵がいないとき, ランダム移動 */
	if (candidate == DIRECTION::NONE) { candidate = static_cast<DIRECTION>(1 << (rand() % 4)); }
	
	/* 移動方向へ進めないとき, 目的地への最短経路を再計算 */
	/* NOTE: ランダム移動時に進めないとき, 右>>下>左>上の探索順序となる. */
	if (ToDirectData(dungeon, candidate, width) == MAPSET::DATA::WALL)
	{
		POS_TYPE min_dist = 9999.0f;
		for (int i = -static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; i <= static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; i++)
		{
			if ((py + i) < 0 || (py + i) > height - 1) continue;
			for (int j = -static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; j <= static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; j++)
			{
				if ((px + j) < 0 || (px + j) > width - 1) continue;
				if (dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROAD) || dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROOM))
				{
					POS_TYPE dist = my_math::Math::Distance<POS_TYPE>(dest_x,dest_y,static_cast<POS_TYPE>(px+j),static_cast<POS_TYPE>(py+i));
					if (dist < min_dist)
					{
						dest_x = static_cast<POS_TYPE>(px + j), dest_y = static_cast<POS_TYPE>(py + i);
						min_dist = dist;
					}
				}
			}
		}
		candidate = GetVector(dest_x, dest_y);
	}

	if (ToDirectData(dungeon, candidate, width) == MAPSET::DATA::PLAYER)
	{
		turn_cost = TURN_MODE::ATTACK;
	}

	return candidate;
}
/* A-STAR探索 */
DIRECTION Enemy::A_STAR(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;

	if (need_research_route)
	{
		need_research_route = false;
		my_math::Vec<int> target_pos = CommonCharacter::SearchTargetCoord(dungeon, width, height, target_id);
		route_pos = CommonCharacter::A_STAR(dungeon,width,height,static_cast<int>(x),static_cast<int>(y),target_pos);
	}
	if(!route_pos.empty())
	{
		my_math::Vec<int> pos = route_pos.top();
		route_pos.pop();
		return GetVector(static_cast<POS_TYPE>(pos.x), static_cast<POS_TYPE>(pos.y));
	}

	return DIRECTION::NONE;
}