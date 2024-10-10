#include "DashState.h"
#include "IdleState.h"
#include "JumpState.h"
#include "HitState.h"
#include "WinState.h"
#include "Player.h"
#include "Input.h"
void DashState::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"Dash.txt"));
	Tag = DASH;
	DashTime = 0.55f;
	Owner->SetPlaySound(L"ROCK_X5_00126.wav"); // Play Dash Effect Sound
}

void DashState::Update()
{
	if (Owner->IsPlayerHit()) // Player Hit
	{
		if (Owner->IsPlayerCanHit()) // if Can Player Hit
		{
			Owner->ChangeState(new HitState);
			return;
		}
	}
	if (Owner->GetPlayerWin())
	{
		Owner->ChangeState(new WinState);
		return;
	}
	Owner->PlayAnimation(); // Play Character Animation
	Owner->PlayDashEffect(); // Play Dash Effect
	if (!Owner->GetLeftWallState() && !Owner->GetRightWallState()) // Check Left/Right Wall Collision
	{
		Owner->PlayerDash(500.f);
	}
	DashTime -= gSecondPerFrame;
	if (DashTime < 0.f) // if Dash End
	{
		Owner->ChangeState(new IdleState);
		return;
	}

	if (GameInput.GetKey('X') == KEY_PUSH) // Dash Attack
	{
		WaitTime = 0.f;
		Owner->ResetChargeState();
		Owner->SetAnimation(Owner->FindSprite(L"ShootDash.txt"));
		Owner->SpawnBullet();
	}
	else if (GameInput.GetKey('X') == KEY_HOLD)
	{
		Owner->UpdateChargingState();//Start Charge
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
			if (WaitTime > 0.8f) //Variables for smoothing the animation connection
			{
				Owner->SetAnimation(Owner->FindSprite(L"Dash.txt"));
			}
		}
	}

	if (GameInput.GetKey('C') == KEY_PUSH)
	{
		Owner->ChangeState(new JumpState);
		return;
	}
}

void DashState::Exit()
{
	Owner->ResetAnimIndex();
	Owner->DashEndEvent();
	Owner = nullptr;
}
