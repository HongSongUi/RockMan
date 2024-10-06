#pragma once
#include "State.h"
class JumpState : public State
{
	float WaitTime;
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

