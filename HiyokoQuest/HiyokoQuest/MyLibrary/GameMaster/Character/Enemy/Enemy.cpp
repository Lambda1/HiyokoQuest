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
	/* ï`âÊÇÃÇΩÇﬂÇ…, ëOâÒÇÃç¿ïWÇï€ë∂ */
	prev_x = x, prev_y = y;
	way = direct;
	/* à⁄ìÆç¿ïWÇÃåvéZ */
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
/* AIèàóù */
/* NOTE: å¯ó¶âªÇÃÇΩÇﬂ,switchï∂ */
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
/* éÄñSîªíË */
void Enemy::JudgeDeath()
{
	if (hp <= 0) {
		death = true;
	}
}
/* AIèàóù */
/* ïWèÄçıìGAI */
DIRECTION Enemy::Standard(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;
	return DIRECTION::NONE;
}
/* ÉoÅ[ÉTÅ[ÉJAI */
DIRECTION Enemy::Berserk(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;
	
	DIRECTION candidate = DIRECTION::NONE;
	int px = static_cast<int>(x), py = static_cast<int>(y);
	int nx = 0, ny = 0;
	int index_x = 0,index_y = 0;
	POS_TYPE dest_x = static_cast<POS_TYPE>(x), dest_y = static_cast<POS_TYPE>(y);
	
	/* éãäEì‡ÇÃìGÇíTçı */
	CommonCharacter::VisualRarnge(&nx, &ny, &index_x, &index_y, static_cast<int>(visual_field), way);
	for (int i = ny; i < index_y; i++)
	{
		if ((py + i) < 0 || (py + i) > height - 1) continue;
		for (int j = nx; j < index_x; j++)
		{
			if ((px + j) < 0 || (px + j) > width - 1) continue;
			if (dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::PLAYER))
			{
				dest_x = static_cast<POS_TYPE>(px + j), dest_y = static_cast<POS_TYPE>(py + i);
				candidate = CommonCharacter::GetVector(x, y, dest_x, dest_y);
				break;
			}
		}
		if (candidate != DIRECTION::NONE) break;
	}
	/* éãäEì‡Ç…ìGÇ™Ç¢Ç»Ç¢Ç∆Ç´, ÉâÉìÉ_ÉÄà⁄ìÆ */
	if (candidate == DIRECTION::NONE) { candidate = static_cast<DIRECTION>(1 << (rand() % 4)); }
	
	/* à⁄ìÆï˚å¸Ç÷êiÇﬂÇ»Ç¢Ç∆Ç´, ñ⁄ìIínÇ÷ÇÃç≈íZåoòHÇçƒåvéZ */
	/* NOTE: ÉâÉìÉ_ÉÄà⁄ìÆéûÇ…êiÇﬂÇ»Ç¢Ç∆Ç´, âE>>â∫>ç∂>è„ÇÃíTçıèáèòÇ∆Ç»ÇÈ. */
	if (CommonCharacter::ToDirectData(x, y, dungeon, candidate, width) == MAPSET::DATA::WALL)
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
		candidate = CommonCharacter::GetVector(x, y, dest_x, dest_y);
	}

	if (CommonCharacter::ToDirectData(x, y, dungeon, candidate, width) == MAPSET::DATA::PLAYER) { turn_cost = TURN_MODE::ATTACK; }

	return candidate;
}
/* A-STARíTçı(çÇÉRÉXÉg) */
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