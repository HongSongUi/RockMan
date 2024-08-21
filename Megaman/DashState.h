#pragma once
#include "State.h"
class DashState : public State
{
	float DashTime;
	float WaitTime;
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

