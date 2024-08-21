#pragma once
#include "State.h"
class HitState : public State
{
	float HitTimer;
public:
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
};

