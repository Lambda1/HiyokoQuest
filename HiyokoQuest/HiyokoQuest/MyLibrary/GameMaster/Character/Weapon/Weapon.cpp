#include "./Weapon.hpp"

Weapon::Weapon() :
	m_weapon(WeaponDef::FIST),
	x(0), y(0), way(DIRECTION::NONE),
	prev_x(0), prev_y(0),
	frame_counter(0)
{
}

Weapon::~Weapon()
{
}

/* ����ɂ��A�j���[�V���� */
bool Weapon::Animation()
{
	bool is_end = false;
	switch (m_weapon)
	{
	case WeaponDef::FIST:
		is_end = Fist(); break;
	case WeaponDef::SWORD:
		break;
	default:
		break;
	}
	return is_end;
}

/* private */
void Weapon::CalcMoveDirect(const POS_TYPE& val)
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

/* �̓�����(�f��)�A�j���[�V���� */
void Weapon::MoveAnimation()
{
	/* X���̈ړ� */
	if (((x - prev_x) < 0 ? (x - prev_x) * -1 : x - prev_x) < M_EPSILON) { prev_x = x; }
	else
	{
		if (x - prev_x < 0) prev_x -= MOVE_RESOlUTION;
		else prev_x += MOVE_RESOlUTION;
	}
	/* Y���̈ړ� */
	if (((y - prev_y) < 0 ? (y - prev_y) * -1 : y - prev_y) < M_EPSILON) { prev_y = y; }
	else
	{
		if (y - prev_y < 0) prev_y -= MOVE_RESOlUTION;
		else prev_y += MOVE_RESOlUTION;
	}
}
/* �f��A�j���[�V�������� */
bool Weapon::Fist()
{
	/*  0�t���[���ڂ̎�, ������ */
	if (frame_counter == 0)
	{
		prev_x = x, prev_y = y;
		pos_stack.push(x);
		pos_stack.push(y);
		CalcMoveDirect(attack_move);
	}
	/* 10�t���[���ڂ̎�, ���̒n�_�֖߂� */
	else if (frame_counter == 10)
	{
		y = pos_stack.top();
		pos_stack.pop();
		x = pos_stack.top();
		pos_stack.pop();
	}
	/* 20�t���[���ڂ̎�, �U�����[�V�����I�� */
	else if (frame_counter > 20)
	{
		std::cout << x << " " << y << std::endl;
		frame_counter = 0;
		return true;
	}
	MoveAnimation();
	frame_counter++;
	return false;
}