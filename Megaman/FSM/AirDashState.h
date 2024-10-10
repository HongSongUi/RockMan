#pragma once
#include "State.h"
class AirDashState : public State
{
	float DashTime; 
	float WaitTime; // Variables for smoothing the animation connection
public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;
};

