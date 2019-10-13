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
	CalcMoveDirect(1.0f);
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
	std::cout << "MAX HP MP: " << max_hp << " " << max_mp << std::endl;
	std::cout << "LV HP MP: " << level << " " << hp << " " << mp << std::endl;
	std::cout << "EXP NEXP: " << exp << " "  << next_level_exp << std::endl;
	std::cout << "ATK DEF: " << power << " " << defence << std::endl;
	std::cout << "DEATH FRIEND: " << death << " " << is_friend << std::endl;
	std::cout << std::endl;
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

		next_level_exp = level * 2;
		
		exp = 0;
		hp = max_hp;
		mp = max_mp;
	}
}
void Player::JudgeDeath()
{
	if (hp <= 0) {
		death = true;
	}
}