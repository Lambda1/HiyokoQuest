#include "./CommonCharacter.hpp"

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

/* A-STARíTçı */
std::stack<my_math::Vec<int>> CommonCharacter::A_STAR(const MAP_TYPE* dungeon, const int& width, const int& height, const int& px, const int& py, const my_math::Vec<int>& target_pos)
{
	std::list<ENEMY_AI::MapCell*> node_list;
	const int size = width * height;
	std::vector<bool> search_map(size, false);
	int now_x = px, now_y = py;
	int counter = 0;
	const int visual_range = static_cast<int>(ENEMY_AI::VISUAL_SIZE::AROUND);

	auto calc_cost = [&](const int& px, const int& py)
	{
		int tmp_x = (target_pos.x - px);
		int tmp_y = (target_pos.y - py);
		return tmp_x > tmp_y ? tmp_x : tmp_y;
	};
	node_list.push_back(new ENEMY_AI::MapCell(now_x, now_y, counter++, calc_cost(now_x, now_y), nullptr));

	ENEMY_AI::MapCell *attract_p = *node_list.begin();
	while (counter < CENSORING_COST && (now_x != target_pos.x || now_y != target_pos.y))
	{
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
					node_list.push_back(new ENEMY_AI::MapCell(now_x + j, now_y + i, counter, calc_cost(now_x, now_y), attract_p));
				}
			}
		}

		attract_p->status = ENEMY_AI::MapCell::STATUS::CLOSE;
		attract_p = GetMaxValueItr(node_list, attract_p);
		now_x = (attract_p)->x, now_y = (attract_p)->y;
		counter++;
	}
	std::stack<my_math::Vec<int>> route_pos;
	for (ENEMY_AI::MapCell* cell = attract_p; cell != nullptr; cell = cell->parent)
	{
		route_pos.push(my_math::Vec<int>(cell->x, cell->y, 0));
	}

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
	double score_min = 9999;
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