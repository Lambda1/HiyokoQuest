#ifndef __STAIR_HPP__
#define __STAIR_HPP__

#include "../Character.hpp"

class Stair : public Character
{
public:
	Stair();
	~Stair();

	void Move(DIRECTION direct) override;
	void Teleport(const int x, const int y) override;

	void Update() override;
};

#endif
