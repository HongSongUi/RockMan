#pragma once
#include "State.h"
class IdleState : public State
{
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

