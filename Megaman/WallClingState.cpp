#include "WallClingState.h"
#include "IdleState.h"
#include "JumpState.h"
#include "HitState.h"
#include "WinState.h"
#include "Player.h"
#include "Input.h"

void WallClingState::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"WallCling.txt"));
	FallingSpeed = 15.f;
	Tag = WALLCLING;
	Owner->UpdateJumpSpeed();
	Owner->SetAnimationEndLoop();
	Owner->SetJumpState(true);
	Owner->SetPlaySound(L"ROCK_X5_00125.wav");
}

void WallClingState::Update()
{
	if (Owner->IsPlayerHit())
	{
		if (Owner->IsPlayerCanHit())
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
	Owner->PlayAnimation();
	Owner->PlayWallEffect();
	Owner->Falling(FallingSpeed);
	if (GameInput.GetKey('X') == KEY_PUSH)
	{
		Owner->SetAnimation(Owner->FindSprite(L"ShootWallCling.txt"));
		Owner->ResetChargeState();
		Owner->SpawnBullet();
		WaitTime = 0.0f;
	}
	else if (GameInput.GetKey('X') == KEY_HOLD)
	{
		Owner->UpdateChargingState();
		if (Owner->GetIsCharge())
		{
			Owner->SetAnimation(Owner->FindSprite(L"WallCling.txt"));
		}
	}
	else if (GameInput.GetKey('X') == KEY_FREE) 
	{
		
		if (Owner->GetIsCharge())
		{
			Owner->SetAnimation(Owner->FindSprite(L"ShootWallCling.txt"));
			Owner->SpawnBullet();
			Owner->ResetChargeState();
		}
		else
		{
			WaitTime += gSecondPerFrame;
			if (WaitTime > 0.5f)
			{
				Owner->SetAnimation(Owner->FindSprite(L"WallCling.txt"));
				WaitTime = 0.0f;
			}
		}
	}
	if (GameInput.GetKey('C') == KEY_PUSH)
	{
		Owner->ChangeState(new JumpState);

		return;
	}
	if (Owner->GetIsGround())
	{
		Owner->SetPlaySound(L"ROCK_X5_00070.wav");
		Owner->ChangeState(new IdleState);
		return;
	}
}

void WallClingState::Exit()
{
	Owner->ResetAnimIndex();
	Owner->SetJumpState(false);
	Owner->ResetEndLoop();
	Owner->WallClingEndEvent();
}
