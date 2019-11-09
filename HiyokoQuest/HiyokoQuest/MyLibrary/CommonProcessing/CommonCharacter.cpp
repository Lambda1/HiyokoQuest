#include "./CommonCharacter.hpp"

CommonCharacter::CommonCharacter()
{
}
CommonCharacter::~CommonCharacter()
{
}

/* 座標間の方向取得 */
DIRECTION CommonCharacter::GetVector(const POS_TYPE &x,const POS_TYPE &y, const POS_TYPE& px, const POS_TYPE& py)
{
	int ang = static_cast<int>(my_math::Math::Angle<POS_TYPE>(x, y, px, py) * 180.0 / my_math::Math::PI);
	for (int i = 0; i < DIRECT_TABLE::table_size; i++)
	{
		if (DIRECT_TABLE::manager_direct[i].m_ang1 <= ang && ang <= DIRECT_TABLE::manager_direct[i].m_ang2)
		{
			return DIRECT_TABLE::manager_direct[i].m_direct;
		}
	}
	return DIRECTION::NONE;
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

/* 視界方向のインデックス取得 */
/* NOTE: 速度を考慮して,switch文で記述. */
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

/* dungeon内のtargetを探索 */
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

/* A-STAR探索 */
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