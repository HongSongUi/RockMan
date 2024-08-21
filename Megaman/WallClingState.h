#pragma once
#include "State.h"
class WallClingState : public State
{
	float FallingSpeed;
	float WaitTime;
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

