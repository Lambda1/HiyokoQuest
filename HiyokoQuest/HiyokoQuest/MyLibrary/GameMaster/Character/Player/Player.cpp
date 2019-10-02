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
void Player::Teleport(const int x, const int y)
{
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
/* private */
/* XVˆ— */
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