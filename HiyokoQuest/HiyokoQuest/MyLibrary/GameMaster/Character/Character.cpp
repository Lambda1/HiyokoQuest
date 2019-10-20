#include "./Character.hpp"

Character::Character() :
	turn_cost(TURN_MODE::NONE),
	chara_state(MAPSET::DATA::NONE),
	x(0), y(0), way(DIRECTION::SOUTH),
	hp(0), mp(0),
	max_hp(0), max_mp(0),
	level(0), power(0), defence(0),
	exp(0), next_level_exp(0),
	death(false), is_friend(false), is_attack(false), attacked_ch(MAPSET::DATA::NONE),
	prev_x(x), prev_y(y),
	frame_counter(0)
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
/* アニメーション */
/* 移動アニメーション */
void Character::MoveAnimation()
{
	/* X軸の移動 */
	if (((x - prev_x) < 0 ? (x - prev_x) * -1 : x - prev_x) < M_EPSILON) { prev_x = x; }
	else
	{
		if (x - prev_x < 0) prev_x -= MOVE_RESOlUTION;
		else prev_x += MOVE_RESOlUTION;
	}
	/* Y軸の移動 */
	if (((y - prev_y) < 0 ? (y - prev_y) * -1 : y - prev_y) < M_EPSILON) { prev_y = y; }
	else
	{
		if (y - prev_y < 0) prev_y -= MOVE_RESOlUTION;
		else prev_y += MOVE_RESOlUTION;
	}
}
/* 攻撃アニメーション */
void Character::AttackAnimation()
{
	/*  0フレーム目の時, 初期化 */
	if (frame_counter == 0)
	{
		pos_stack.push(x);
		pos_stack.push(y);
		CalcMoveDirect(attack_move);
	}
	/* 10フレーム目の時, 元の地点へ戻る */
	else if (frame_counter == 10)
	{
		y = pos_stack.top();
		pos_stack.pop();
		x = pos_stack.top();
		pos_stack.pop();
	}
	/* 20フレーム目の時, 攻撃モーション終了 */
	else if (frame_counter > 20)
	{
		frame_counter = 0;
		turn_cost = TURN_MODE::END;
		return;
	}
	MoveAnimation();
	frame_counter++;
}

/* private */
void Character::CalcMoveDirect(const POS_TYPE& val)
{
	switch (way)
	{
	case DIRECTION::EAST:
		x += val; break;
	case DIRECTION::WEST:
		x -= val; break;
	case DIRECTION::SOUTH:
		y += val; break;
	case DIRECTION::NORTH:
		y -= val; break;
	case DIRECTION::SOUTH_EAST:
		x += val; y += val; break;
	case DIRECTION::SOUTH_WEST:
		x -= val; y += val; break;
	case DIRECTION::NORTH_EAST:
		x += val; y -= val; break;
	case DIRECTION::NORTH_WEST:
		x -= val; y -= val; break;
	default:
		break;
	}
}