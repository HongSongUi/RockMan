#pragma once
#include "State.h"
class WinState : public State
{
public:
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
};