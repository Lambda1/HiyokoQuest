#ifndef __STAIR_HPP__
#define __STAIR_HPP__

#include "../Character.hpp"

class Stair : public Character
{
public:
	Stair();
	~Stair();

	void Move(DIRECTION direct) override;
	void Teleport(const POS_TYPE x, const POS_TYPE y) override;

	void Update() override;

	inline int GiveEXP() override { return exp; } /* 階段は倒されないので, 呼び出さない */
};

#endif