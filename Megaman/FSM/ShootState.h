#pragma once
#include "State.h"
class ShootState :  public State
{
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
	void UpdateState();
};

