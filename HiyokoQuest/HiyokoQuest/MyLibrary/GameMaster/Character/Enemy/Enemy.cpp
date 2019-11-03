#include "./Enemy.hpp"

Enemy::Enemy() : 
	ai_mode(ENEMY_AI::MODE::STANDARD), visual_field(ENEMY_AI::VISUAL_SIZE::NORMAL)
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

	InitTableAI();
}

Enemy::Enemy(const float &up_rate,const MAPSET::DATA &id) :
	ai_mode(ENEMY_AI::MODE::BERSERK), visual_field(ENEMY_AI::VISUAL_SIZE::NORMAL)
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

	InitTableAI();
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
DIRECTION Enemy::AI_Move(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	if (manage_ai_table.find(ai_mode) != manage_ai_table.end())
	{ return (this->*manage_ai_table[ai_mode])(dungeon, width, height); }
	return DIRECTION::NONE;
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
	
	int cand_x = 0, cand_y = 0;
	int px = static_cast<int>(x), py = static_cast<int>(y);
	int nx = 0, ny = 0;
	int index_x = 0,index_y = 0;
	VisualRarnge(&nx, &ny, &index_x, &index_y, static_cast<int>(visual_field));
	for (int i = ny; i < index_y; i++)
	{
		if ((py + i) < 0 || (py + i) > height - 1) continue;
		for (int j = nx; j < index_x; j++)
		{
			if ((px + j) < 0 || (px + j) > width - 1) continue;
			std::cout << (int)dungeon[(py + i) * width + (px + j)];
			if (dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROAD))
			{
				cand_x = (px + j), cand_y = (py + i);
			}
			else if (dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::PLAYER))
			{
				cand_x = (px + j), cand_y = (py + i);
				return GetVector(static_cast<POS_TYPE>(cand_x), static_cast<POS_TYPE>(cand_y));
			}
		}
		std::cout << std::endl;
	}
	return DIRECTION::EAST;
}