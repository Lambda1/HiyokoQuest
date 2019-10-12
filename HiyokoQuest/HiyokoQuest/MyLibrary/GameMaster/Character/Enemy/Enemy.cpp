#include "./Enemy.hpp"

Enemy::Enemy()
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

Enemy::~Enemy()
{
}

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
	exp += 100;
	JudgeDeath();
}

/* private */
/* 更新処理 */
void Enemy::JudgeDeath()
{
	if (hp <= 0) {
		death = true;
	}
}