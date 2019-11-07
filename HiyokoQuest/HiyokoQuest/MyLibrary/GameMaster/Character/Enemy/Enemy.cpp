#include "./Enemy.hpp"

Enemy::Enemy() :
	ai_mode(ENEMY_AI::MODE::STANDARD), visual_field(ENEMY_AI::VISUAL_SIZE::NORMAL),
	need_research_route(true)
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
	need_research_route(true)
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
	/* �`��̂��߂�, �O��̍��W��ۑ� */
	prev_x = x, prev_y = y;
	way = direct;
	/* �ړ����W�̌v�Z */
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
/* AI���� */
DIRECTION Enemy::AI_Mode(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	/* EnemyAI��, �������̂��߃e�[�u�������Ȃ� */
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

/* �X�V���� */
/* ���S���� */
void Enemy::JudgeDeath()
{
	if (hp <= 0) {
		death = true;
	}
}

/* AI���� */
/* �W�����GAI */
DIRECTION Enemy::Standard(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;
	return DIRECTION::NONE;
}
/* �o�[�T�[�JAI */
DIRECTION Enemy::Berserk(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;
	
	DIRECTION candidate = DIRECTION::NONE;
	int px = static_cast<int>(x), py = static_cast<int>(y);
	int nx = 0, ny = 0;
	int index_x = 0,index_y = 0;
	POS_TYPE dest_x = static_cast<POS_TYPE>(x), dest_y = static_cast<POS_TYPE>(y);
	
	/* ���E���̓G��T�� */
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
	/* ���E���ɓG�����Ȃ��Ƃ�, �����_���ړ� */
	if (candidate == DIRECTION::NONE) { candidate = static_cast<DIRECTION>(1 << (rand() % 4)); }
	
	/* �ړ������֐i�߂Ȃ��Ƃ�, �ړI�n�ւ̍ŒZ�o�H���Čv�Z */
	/* NOTE: �����_���ړ����ɐi�߂Ȃ��Ƃ�, �E>>��>��>��̒T�������ƂȂ�. */
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
/* A-STAR�T�� */
DIRECTION Enemy::A_STAR(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	turn_cost = TURN_MODE::MOVE;

	if (need_research_route)
	{
		std::list<ENEMY_AI::MapCell*> node_list;
		calc_cost = [&](const int &px,const int &py)
		{
			int tmp_x = (target_pos.x - px);
			int tmp_y = (target_pos.y - py);
			return tmp_x > tmp_y ? tmp_x : tmp_y;
		};
		need_research_route = false;
		const int size = width * height;
		bool* search_map = new bool[size];
		for (int i = 0; i < size; i++) search_map[i] = false;

		int now_x = static_cast<int>(x), now_y = static_cast<int>(y);
		const int ran_x = 3, ran_y = 3;
		int counter = 0;
		node_list.push_back(new ENEMY_AI::MapCell(now_x, now_y, counter++, calc_cost(now_x, now_y), nullptr));
		auto attract_itr = node_list.begin();
		
		while (now_x != target_pos.x || now_y != target_pos.y)
		{
			for (int i = -ran_y / 2; i <= ran_y / 2; i++)
			{
				if ((i + now_y) < 0 || (i + now_y) > height) continue;
				for (int j = -ran_x / 2; j <= ran_x / 2; j++)
				{
					if ((j + now_x) < 0 || (j + now_y) > width) continue;
					if (search_map[(i + now_y) * width + (j + now_x)]) continue;
					if (dungeon[(i + now_y) * width + (j + now_x)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROAD) || dungeon[(i + now_y) * width + (j + now_x)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROOM))
					{
						search_map[(i + now_y) * width + (j + now_x)] = true;
						node_list.push_back(new ENEMY_AI::MapCell(now_x+j, now_y+i, counter, calc_cost(now_x, now_y), *attract_itr));
					}
				}
			}

			int score_min = 9999;
			(*attract_itr)->status = ENEMY_AI::MapCell::STATUS::CLOSE;
			for (auto itr = node_list.begin(); itr != node_list.end(); ++itr)
			{
				if ((*itr)->status == ENEMY_AI::MapCell::STATUS::CLOSE) continue;
				if ((*itr)->GetScore() < score_min) {
					attract_itr = itr;
					score_min = (*itr)->GetScore();
				}
			}
			now_x = (*attract_itr)->x, now_y = (*attract_itr)->y;

			counter++;
		}

		for (ENEMY_AI::MapCell* cell = *attract_itr;cell != nullptr;cell = cell->parent)
		{
			route_pos.push(my_math::Vec<int>(cell->x, cell->y, 0));
		}

		delete[] search_map;
		for (auto itr = node_list.begin(); itr != node_list.end();)
		{
			if (*itr)
			{
				delete *itr;
				itr = node_list.erase(itr);
				continue;
			}
			++itr;
		}
	}

	if (!route_pos.empty())
	{
		my_math::Vec<int> pos = route_pos.top();
		route_pos.pop();
		return GetVector(static_cast<POS_TYPE>(pos.x), static_cast<POS_TYPE>(pos.y));
	}else
	{
		need_research_route = true;
	}

	return DIRECTION::NONE;
}