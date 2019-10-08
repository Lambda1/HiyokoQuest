#include "./Character.hpp"

Character::Character() :
	turn_cost(TURN_MODE::NONE),
	chara_state(MAPSET::DATA::NONE),
	x(0), y(0), way(DIRECTION::SOUTH),
	hp(0), mp(0),
	max_hp(0), max_mp(0),
	level(0), power(0), defence(0),
	exp(0), next_level_exp(0),
	death(false), is_friend(false), is_attack(false),
	prev_x(x), prev_y(y)
{
}

Character::~Character()
{
}

/* 初期化処理 */
void Character::InitPos(const POS_TYPE x, const POS_TYPE y)
{
	this->x = prev_x = x;
	this->y = prev_y = y;
}
void Character::InitStatus(const int level, const int power, const int defence)
{
	this->level = level;
	this->power = power;
	this->defence = defence;
}
void Character::InitMAX(const int max_hp, const int max_mp)
{
	this->max_hp = max_hp;
	this->max_mp = max_mp;
}
/* ステータス処理 */
void Character::Damaged(const int damaged_value)
{
	hp -= damaged_value;
	if (hp < 0) hp = 0;
}
int Character::Attack(const int damaged_side_defence)
{
	return ((power-damaged_side_defence) >= 0 ? (power - damaged_side_defence) : 0);
}
void Character::UseMP(const int used_mp)
{
	mp -= used_mp;
	if (mp < 0) mp = 0;
}
void Character::GetEXP(const int get_exp)
{
	exp += get_exp;
}