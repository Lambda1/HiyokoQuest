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
DIRECTION Enemy::AI_Move(const MAP_TYPE* dungeon, const int& width, const int& height)
{
	if (manage_ai_table.find(ai_mode) != manage_ai_table.end())
	{ return (this->*manage_ai_table[ai_mode])(dungeon, width, height); }
	return DIRECTION::NONE;
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
	for (int i = -static_cast<int>(visual_field)/2; i < static_cast<int>(visual_field) / 2; i++)
	{
		if ((py + i) < 0 || (py + i) > height - 1) continue;
		for (int j = -1; j < static_cast<int>(visual_field); j++)
		{
			if ((px + j) < 0 || (px + j) > width - 1) continue;
			if (dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROOM) || dungeon[(py + i) * width + (px + j)] == static_cast<MAP_TYPE>(MAPSET::DATA::ROAD))
			{
				candidate = GetVector(static_cast<POS_TYPE>(px + j), static_cast<POS_TYPE>(py + i));
			}
		}
	}

	return candidate;
}