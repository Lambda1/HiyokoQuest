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
	/* �`��̂��߂�, �O��̍��W��ۑ� */
	prev_x = x, prev_y = y;
	way = direct;
	/* �ړ����W�̌v�Z */
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
}

/* private */
/* �X�V���� */
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