#include "./Enemy.hpp"

Enemy::Enemy() : 
	ai_mode(ENEMY_AI::MODE::STANDARD)
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
	ai_mode(ENEMY_AI::MODE::STANDARD)
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
DIRECTION Enemy::AI_Move(const MAP_TYPE* dungeon)
{
	return Standard(dungeon);
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
/* �W�����G */
DIRECTION Enemy::Standard(const MAP_TYPE* dungeon)
{
	turn_cost = TURN_MODE::MOVE;
	return DIRECTION::EAST;
}