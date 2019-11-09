#include "./CommonCharacter.hpp"

CommonCharacter::CommonCharacter()
{
}
CommonCharacter::~CommonCharacter()
{
}

POS_TYPE CommonCharacter::TransAngle(const DIRECTION& way)
{
	POS_TYPE ang = 0.0f;
	switch (way)
	{
	case DIRECTION::SOUTH:
		ang = DIRECT_TABLE::ANG_DEG_8 * 0.0f; break;
	case DIRECTION::SOUTH_EAST:
		ang = DIRECT_TABLE::ANG_DEG_8 * 1.0f; break;
	case DIRECTION::EAST:
		ang = DIRECT_TABLE::ANG_DEG_8 * 2.0f; break;
	case DIRECTION::NORTH_EAST:
		ang = DIRECT_TABLE::ANG_DEG_8 * 3.0f; break;
	case DIRECTION::NORTH:
		ang = DIRECT_TABLE::ANG_DEG_8 * 4.0f; break;
	case DIRECTION::NORTH_WEST:
		ang = DIRECT_TABLE::ANG_DEG_8 * 5.0f; break;
	case DIRECTION::WEST:
		ang = DIRECT_TABLE::ANG_DEG_8 * 6.0f; break;
	case DIRECTION::SOUTH_WEST:
		ang = DIRECT_TABLE::ANG_DEG_8 * 7.0f; break;
	default:
		break;
	}
	return ang;
}

MAPSET::DATA CommonCharacter::ToDirectData(POS_TYPE px, POS_TYPE py, const MAP_TYPE* dungeon, const DIRECTION& direct, const int& width)
{
	switch (direct)
	{
	case DIRECTION::EAST:
		px++; break;
	case DIRECTION::WEST:
		px--; break;
	case DIRECTION::SOUTH:
		py++; break;
	case DIRECTION::NORTH:
		py--; break;
	case DIRECTION::SOUTH_EAST:
		px++; py++; break;
	case DIRECTION::SOUTH_WEST:
		px--; py++; break;
	case DIRECTION::NORTH_EAST:
		px++; py--; break;
	case DIRECTION::NORTH_WEST:
		px--; py--; break;
	default:
		break;
	}
	return static_cast<MAPSET::DATA>(dungeon[static_cast<int>(py) * width + static_cast<int>(px)]);
}

/* éãäEï˚å¸ÇÃÉCÉìÉfÉbÉNÉXéÊìæ */
/* NOTE: ë¨ìxÇçló∂ÇµÇƒ,switchï∂Ç≈ãLèq. */
void CommonCharacter::VisualRarnge(int* x, int* y, int* index_x, int* index_y, const int& weight, const DIRECTION& way)
{
	switch (way)
	{
	case DIRECTION::EAST:
	case DIRECTION::NORTH_EAST:
	case DIRECTION::SOUTH_EAST:
		*x = -1, *y = -weight / 2;
		*index_x = weight, * index_y = weight / 2;
		break;
	case DIRECTION::WEST:
	case DIRECTION::NORTH_WEST:
	case DIRECTION::SOUTH_WEST:
		*x = -weight, * y = -weight / 2;
		*index_x = 2, *index_y = weight / 2;
		break;
	case DIRECTION::SOUTH:
		*x = -weight / 2, *y = -1;
		*index_x = weight / 2, *index_y = weight;
		break;
	case DIRECTION::NORTH:
		*x = -weight / 2, *y = -weight;
		*index_x = weight / 2, *index_y = 2;
		break;
	default:
		break;
	}
}

/* dungeonì‡ÇÃtargetÇíTçı */
my_math::Vec<int> CommonCharacter::SearchTargetCoord(const MAP_TYPE* dungeon, const int& width, const int& height, const MAPSET::DATA& target)
{
	my_math::Vec<int> target_pos(-1,-1,0);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (dungeon[i * width + j] == static_cast<MAP_TYPE>(target))
			{
				target_pos.Set(j,i);
				return target_pos;
			}
		}
	}
	return target_pos;
}
my_math::Vec<int> CommonCharacter::SearchTargetCoord(const MAP_TYPE* dungeon, const my_math::Vec<int>& start, const my_math::Vec<int>& end,const my_math::Vec<int> &ch_pos,const int &width,const int &height, const MAPSET::DATA& target)
{
	for (int i = start.y; i < end.y; i++)
	{
		if ((ch_pos.y + i) < 0 || (ch_pos.y + i) > height - 1) continue;
		for (int j = start.x; j < end.x; j++)
		{
			if ((ch_pos.x + j) < 0 || (ch_pos.x + j) > width - 1) continue;
			if (dungeon[(ch_pos.y + i) * width + (ch_pos.x + j)] == static_cast<MAP_TYPE>(target))
			{
				return my_math::Vec<int>(ch_pos.x + j, ch_pos.y + i,0);
			}
		}
	}
	return my_math::Vec<int>(-1,-1,0);
}

/* A-STARíTçı */
std::stack<my_math::Vec<int>> CommonCharacter::A_STAR(const MAP_TYPE* dungeon, const int& width, const int& height, const int& px, const int& py, const my_math::Vec<int>& target_pos)
{
	std::list<ENEMY_AI::MapCell*> node_list;
	const int size = width * height;
	std::vector<bool> search_map(size, false);
	int now_x = px, now_y = py;
	int counter = 0;

	node_list.push_back(new ENEMY_AI::MapCell(now_x, now_y, counter++, CalcCost_A_STAR(target_pos, now_x, now_y), nullptr));

	ENEMY_AI::MapCell *attract_p = *node_list.begin();
	while (counter < CENSORING_COST && (now_x != target_pos.x || now_y != target_pos.y))
	{
		attract_p->status = ENEMY_AI::MapCell::STATUS::CLOSE;
		for (int i = -visual_range / 2; i <= visual_range / 2; i++)
		{
			if ((i + now_y) < 0 || (i + now_y) > height) continue;
			for (int j = -visual_range / 2; j <= visual_range / 2; j++)
			{
				int index = (i + now_y) * width + (j + now_x);
				if ((j + now_x) < 0 || (j + now_y) > width) continue;
				if (search_map[index]) continue;
				if (dungeon[index] == static_cast<MAP_TYPE>(MAPSET::DATA::ROAD) ||
					dungeon[index] == static_cast<MAP_TYPE>(MAPSET::DATA::ROOM) ||
					my_math::Vec<int>(j+now_x,i+now_y,0) == target_pos)
				{
					search_map[index] = true;
					node_list.push_back(new ENEMY_AI::MapCell(now_x + j, now_y + i, counter, CalcCost_A_STAR(target_pos,now_x, now_y), attract_p));
				}
			}
		}
		attract_p = GetMaxValueItr(node_list, attract_p);
		now_x = (attract_p)->x, now_y = (attract_p)->y;
		counter++;
	}

	/* íTçıë≈ÇøêÿÇËÇÃèÍçá, åªéûì_Ç≈ÇÃç≈ìKâÇíTçı */
	if (counter >= CENSORING_COST) { attract_p = SearchMinHuristicCost(node_list); }

	/* stackÇ…íTçıåoòHÇpush */
	std::stack<my_math::Vec<int>> route_pos;
	for (ENEMY_AI::MapCell* cell = attract_p; cell->parent != nullptr; cell = cell->parent) { route_pos.push(my_math::Vec<int>(cell->x, cell->y, 0)); }

	for (auto itr = node_list.begin(); itr != node_list.end();)
	{
		if (*itr)
		{
			delete* itr;
			itr = node_list.erase(itr);
			continue;
		}
		++itr;
	}

	return route_pos;
}
ENEMY_AI::MapCell* CommonCharacter::GetMaxValueItr(const std::list<ENEMY_AI::MapCell*>& node_list, ENEMY_AI::MapCell* now_itr)
{
	ENEMY_AI::MapCell *attract_itr = now_itr;
	double score_min = 99999;
	for (auto itr = node_list.begin(); itr != node_list.end(); ++itr)
	{
		if ((*itr)->status == ENEMY_AI::MapCell::STATUS::CLOSE) continue;
		if ((*itr)->GetScoreASTAR() <= score_min)
		{
			attract_itr = *itr;
			score_min = (*itr)->GetScoreASTAR();
		}
	}
	return attract_itr;
}