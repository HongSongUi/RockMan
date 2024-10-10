#include "AirDashState.h"
#include "JumpState.h"
#include "Player.h"
#include "HitState.h"
#include "WinState.h"
#include "Input.h"
void AirDashState::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"Dash.txt"));
	Tag = AIRDASH;
	DashTime = 0.55f;
	Owner->SetPlaySound(L"ROCK_X5_00126.wav");
}

void AirDashState::Update()
{
	if (Owner->IsPlayerHit()) // If Player Hit
	{
		if (Owner->IsPlayerCanHit()) // Check Can Player Hit 
		{
			Owner->ChangeState(new HitState);
			return;
		}
	}
	if (Owner->GetPlayerWin()) // If Player Win
	{
		Owner->ChangeState(new WinState); // Change Win State
		return;
	}
	Owner->PlayAnimation();
	Owner->PlayDashEffect();
	DashTime -= gSecondPerFrame;
	if (!Owner->GetLeftWallState() && !Owner->GetRightWallState()) // Check Left/Right Wall Collision
	{
		Owner->PlayerDash(500.f); // Dash (Speed)
	}

	if (DashTime < 0.f) // Dash End
	{
		Owner->ChangeState(new JumpState);
		return;
	}

	if (GameInput.GetKey('X') == KEY_PUSH) 
	{
		WaitTime = 0.f;
		Owner->ResetChargeState(); // Charge Time = 0
		Owner->SetAnimation(Owner->FindSprite(L"ShootDash.txt"));
		Owner->SpawnBullet();
	}
	else if (GameInput.GetKey('X') == KEY_HOLD)
	{
		Owner->UpdateChargingState(); // Charge Start
		if (Owner->GetIsCharge()) 
		{
			Owner->SetAnimation(Owner->FindSprite(L"Dash.txt"));
		}
	}
	else if (GameInput.GetKey('X') == KEY_FREE)
	{

		if (Owner->GetIsCharge())
		{
			Owner->SetAnimation(Owner->FindSprite(L"ShootDash.txt"));
			Owner->SpawnBullet();
			Owner->ResetChargeState();
		}
		else
		{
			WaitTime += gSecondPerFrame;
			if (WaitTime > 0.8f)
			{
				Owner->SetAnimation(Owner->FindSprite(L"Dash.txt"));
			}
		}
	}
}

void AirDashState::Exit()
{
	Owner->SetFallingAnim();
	Owner->DashEndEvent();
	Owner = nullptr;
}
