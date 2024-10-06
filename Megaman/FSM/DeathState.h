#pragma once
#include "State.h"
class DeathState : public State
{
	float WaitTimer;
public:
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
};

