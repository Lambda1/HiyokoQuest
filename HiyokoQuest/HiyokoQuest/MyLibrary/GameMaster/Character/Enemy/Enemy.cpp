#include "./Enemy.hpp"

Enemy::Enemy() :
	enemy_type(MAPSET::DATA::ENEMY1)
{
	chara_state = MAPSET::DATA::ENEMY;

	hp = max_hp = first_hp;
	mp = max_mp = first_mp;
	level = first_level;
	power = first_power;
	defence = first_defence;
	next_level_exp = first_next_exp;
	
	is_friend = false;
}

Enemy::Enemy(const float &up_rate,const MAPSET::DATA &id) :
	enemy_type(id)
{
	chara_state = MAPSET::DATA::ENEMY;

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

/* private */
/* �X�V���� */
void Enemy::JudgeDeath()
{
	if (hp <= 0) {
		death = true;
	}
}