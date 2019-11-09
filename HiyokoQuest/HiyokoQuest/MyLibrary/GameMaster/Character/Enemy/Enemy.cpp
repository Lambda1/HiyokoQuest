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
/* NOTE: 効率化のため,switch文 */
DIRECTION Enemy::AI_Mode(const MAP_TYPE* dungeon, const int& width, const int& height)
{
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
	my_math::Vec<int> pos(static_cast<int>(x), static_cast<int>(y), 0);
	my_math::Vec<int> start,end;
	
	/* 視界内の敵を探索 */
	CommonCharacter::VisualRarnge(&start.x, &start.y, &end.x, &end.y, static_cast<int>(visual_field), way);
	my_math::Vec<int> dest = CommonCharacter::SearchTargetCoord(dungeon,start,end,pos, width, height, target_id);
	candidate = candidate = CommonCharacter::GetVector<int>(pos.x, pos.y, dest.x, dest.y);

	/* 視界内に敵がいないとき, ランダム移動 */
	if (candidate == DIRECTION::NONE) { candidate = static_cast<DIRECTION>(1 << (rand() % 4)); }
	
	/* 移動方向へ進めないとき, 目的地への最短経路を再計算 */
	/* NOTE: ランダム移動時に進めないとき, 右>>下>左>上の探索順序となる. */
	if (CommonCharacter::ToDirectData(x, y, dungeon, candidate, width) == MAPSET::DATA::WALL)
	{
		double min_dist = 9999.0f;
		for (int i = -static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; i <= static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; i++)
		{
			if ((pos.y + i) < 0 || (pos.y + i) > height - 1) continue;
			for (int j = -static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; j <= static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND) / 2; j++)
			{
				if ((pos.x + j) < 0 || (pos.x + j) > width - 1) continue;
				if (dungeon[(pos.y + i) * width + (pos.x + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROAD) || dungeon[(pos.y + i) * width + (pos.x + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROOM))
				{
					double dist = my_math::Math::Distance<POS_TYPE>(static_cast<POS_TYPE>(dest.x),static_cast<POS_TYPE>(dest.y),static_cast<POS_TYPE>(pos.x+j),static_cast<POS_TYPE>(pos.y+i));
					if (dist < min_dist)
					{
						dest.Set(pos.x+j,pos.y+i);
						min_dist = dist;
					}
				}
			}
		}
		candidate = CommonCharacter::GetVector<int>(pos.x, pos.y, dest.x, dest.y);
	}

	/* 進行方向にtargetがいる時, 移動せずに攻撃する. */
	if (CommonCharacter::ToDirectData(x, y, dungeon, candidate, width) == target_id) { turn_cost = TURN_MODE::ATTACK; }

	return candidate;
}
/* A-STAR探索(高コスト) */
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
		return CommonCharacter::GetVector(x, y, static_cast<POS_TYPE>(pos.x), static_cast<POS_TYPE>(pos.y));
	}
	return DIRECTION::NONE;
}