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
	ai_mode(ENEMY_AI::MODE::A_STAR), visual_field(ENEMY_AI::VISUAL_SIZE::SMALL),
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
	/* •`‰æ‚Ì‚½‚ß‚É, ‘O‰ñ‚ÌÀ•W‚ğ•Û‘¶ */
	prev_x = x, prev_y = y;
	way = direct;
	/* ˆÚ“®À•W‚ÌŒvZ */
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
/* AIˆ— */
/* NOTE: Œø—¦‰»‚Ì‚½‚ß,switch•¶ */
DIRECTION Enemy::AI_Mode(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	DIRECTION candidate = DIRECTION::NONE;
	switch (ai_mode)
	{
	case ENEMY_AI::MODE::STANDARD:
		candidate = Standard(dungeon, width, height); break;
	case ENEMY_AI::MODE::BERSERK:
		candidate = Berserk(dungeon, width, height); break;
	case ENEMY_AI::MODE::A_STAR:
		candidate = A_STAR(dungeon, width, height); break;
	default:
		break;
	}
	if (candidate != DIRECTION::NONE) way = candidate;

	/* is•ûŒü‚Étarget‚ª‚¢‚é, ˆÚ“®‚¹‚¸‚ÉUŒ‚‚·‚é. */
	//if (isTargetToDirect(dungeon, width, way)) { turn_cost = TURN_MODE::ATTACK; }
	DIRECTION tmp = CommonCharacter::isNeighborTarget(dungeon,width,height,my_math::Vec<int>((int)x,(int)y,0),target_id);
	if (tmp != DIRECTION::NONE)
	{
		turn_cost = TURN_MODE::ATTACK;
		way = tmp;
	}
	std::cout << (int)tmp << std::endl;
	return candidate;
}

/* private */
/* €–S”»’è */
void Enemy::JudgeDeath()
{
	if (hp <= 0) {
		death = true;
	}
}
/* AIˆ— */
/* •W€õ“GAI */
DIRECTION Enemy::Standard(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;

	my_math::Vec<int> pos(static_cast<int>(x), static_cast<int>(y), 0);
	my_math::Vec<int> start, end;

	/* ‹ŠE“à‚Ìtarget‚ğ’Tõ */
	CommonCharacter::VisualRarnge(&start.x, &start.y, &end.x, &end.y, static_cast<int>(visual_field), way);
	my_math::Vec<int> dest = CommonCharacter::SearchTargetCoord(dungeon, start, end, pos, width, height, target_id);
	DIRECTION candidate = CommonCharacter::GetVector<int>(pos.x, pos.y, dest.x, dest.y);

	/* ‹ŠE“à‚Étarget‚ª‚¢‚È‚¢‚Æ‚«, ‰Eè’Tõ */
	if (candidate == DIRECTION::NONE)
	{

	}

	return candidate;
}
/* ƒo[ƒT[ƒJAI */
DIRECTION Enemy::Berserk(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;
	
	my_math::Vec<int> pos(static_cast<int>(x), static_cast<int>(y), 0);
	my_math::Vec<int> start,end;
	
	/* ‹ŠE“à‚Ì“G‚ğ’Tõ */
	CommonCharacter::VisualRarnge(&start.x, &start.y, &end.x, &end.y, static_cast<int>(visual_field), way);
	my_math::Vec<int> dest = CommonCharacter::SearchTargetCoord(dungeon,start,end,pos, width, height, target_id);
	DIRECTION candidate = CommonCharacter::GetVector<int>(pos.x, pos.y, dest.x, dest.y);

	/* ‹ŠE“à‚É“G‚ª‚¢‚È‚¢‚Æ‚«, ƒ‰ƒ“ƒ_ƒ€ˆÚ“® */
	if (candidate == DIRECTION::NONE) { candidate = static_cast<DIRECTION>(1 << (rand() % 4)); }
	
	/* ˆÚ“®•ûŒü‚Öi‚ß‚È‚¢‚Æ‚«, –Ú“I’n‚Ö‚ÌÅ’ZŒo˜H‚ğÄŒvZ */
	/* NOTE: ƒ‰ƒ“ƒ_ƒ€ˆÚ“®‚Éi‚ß‚È‚¢‚Æ‚«, ‰E>>‰º>¶>ã‚Ì’Tõ‡˜‚Æ‚È‚é. */
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

	return candidate;
}
/* A-STAR’Tõ(‚ƒRƒXƒg) */
DIRECTION Enemy::A_STAR(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;

	/* Œo˜H‚ÌÄŒvZ */
	if (route_pos.empty()) need_research_route = true;
	/* Œo˜HŒvZ */
	if (need_research_route)
	{
		need_research_route = false;
		my_math::Vec<int> target_pos = CommonCharacter::SearchTargetCoord(dungeon, width, height, target_id);
		route_pos = CommonCharacter::A_STAR(dungeon,width,height,static_cast<int>(x),static_cast<int>(y),target_pos);
	}
	/* Œo˜H‚É‰ˆ‚Á‚Äi‚Ş */
	if(!route_pos.empty())
	{
		my_math::Vec<int> pos = route_pos.top();
		DIRECTION candidate = CommonCharacter::GetVector(x, y, static_cast<POS_TYPE>(pos.x), static_cast<POS_TYPE>(pos.y));
		/* ˆÚ“®‰Â”\, stack‚ğpop */
		if (CommonCharacter::ToDirectData(x, y, dungeon, candidate, width) == MAPSET::DATA::ROOM ||
			CommonCharacter::ToDirectData(x, y, dungeon, candidate, width) == MAPSET::DATA::ROAD)
		{
			route_pos.pop();
			return candidate;
		}
		/* ˆÚ“®•s‰Â”\, Œo˜H‚ÌÄŒvZ */
		need_research_route = true;
	}

	return DIRECTION::NONE;
}