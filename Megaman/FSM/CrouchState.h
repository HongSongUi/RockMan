#pragma once
#include "State.h"
class CrouchState : public State
{
	float WaitTime;
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

