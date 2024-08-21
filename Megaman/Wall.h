#pragma once
#include "Object2D.h"
class Wall : public Object2D
{
public:
	Rect LeftWall;
	Rect Ground;
	Rect RightWall;
public:
	bool Init() override;
};

