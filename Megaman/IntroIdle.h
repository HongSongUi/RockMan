#pragma once
#include "State.h"
class IntroIdle : public State
{
	bool IsGround;
	bool IsPlaySound;
public:
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
};

