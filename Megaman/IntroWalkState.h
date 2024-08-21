#pragma once
#include "State.h"
class IntroWalkState : public State
{
public:
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
};

