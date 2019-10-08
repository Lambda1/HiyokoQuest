#include "./Player.hpp"

Player::Player()
{
	chara_state = MAPSET::DATA::PLAYER;

	hp = max_hp = first_hp;
	mp = max_mp = first_mp;
	level = first_level;
	power = first_power;
	defence = first_defence;
	next_level_exp = first_next_exp;
	
	is_friend = true;
}

Player::~Player()
{
}

void Player::Move(DIRECTION direct)
{
	/* 描画のために, 前回の座標を保存 */
	prev_x = x, prev_y = y;
	way = direct;
	/* 移動座標の計算 */
	switch (direct)
	{
	case DIRECTION::EAST:
		x++; break;
	case DIRECTION::WEST:
		x--; break;
	case DIRECTION::SOUTH:
		y++; break;
	case DIRECTION::NORTH:
		y--; break;
	case DIRECTION::SOUTH_EAST:
		x++; y++; break;
	case DIRECTION::SOUTH_WEST:
		x--; y++; break;
	case DIRECTION::NORTH_EAST:
		x++; y--; break;
	case DIRECTION::NORTH_WEST:
		x--; y--; break;
	default:
		break;
	}
}
void Player::Teleport(const POS_TYPE x, const POS_TYPE y)
{
	prev_x = x, prev_y = y;
	this->x = x;
	this->y = y;
}
void Player::Update()
{
	LevelUp();
	JudgeDeath();
	
	std::cout << "Player Status" << std::endl;
	std::cout << "Position: " << x << " " << y << std::endl;
	std::cout << "MAX: " << max_hp << " " << max_mp << std::endl;
	std::cout << "L H M: " << level << " " << hp << " " << mp << std::endl;
	std::cout << "EXP NEXP: " << exp << " "  << next_level_exp << std::endl;
	std::cout << "D F: " << death << " " << is_friend << std::endl;
	std::cout << std::endl;
}

/* 描画用処理 */
/* 移動アニメーション */
void Player::MoveAnimation()
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

	/* 移動は完了しているか? */
	if (x == prev_x && y == prev_y) { turn_cost = TURN_MODE::END; return; }

	turn_cost = TURN_MODE::MOVE;
}

/* private */
/* 更新処理 */
void Player::LevelUp()
{
	if (exp >= next_level_exp)
	{
		max_hp++;
		max_mp++;
		level++;
		power++;
		defence++;

		next_level_exp = level * 10;
		
		exp = 0;
		hp = max_hp;
		mp = max_mp;
	}
}
void Player::JudgeDeath()
{
	if (hp == 0) {
		death = true;
	}
}