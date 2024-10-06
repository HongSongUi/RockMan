#include "JumpState.h"
#include "IdleState.h"
#include "DashState.h"
#include "WallClingState.h"
#include "HitState.h"
#include "AirDashState.h"
#include "WinState.h"
#include "Input.h"
#include "Player.h"
void JumpState::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"Jump.txt"));
	Tag = JUMP;
	Owner->SetJumpState(true);
	Owner->UpdateJumpSpeed();
	if (Owner->GetPrevState() == WALLCLING)
	{
		Owner->SetPlaySound(L"ROCK_X5_00206.wav");
	}
	else if (Owner->GetPrevState() != AIRDASH)
	{
		Owner->SetPlaySound(L"ROCK_X5_00764.wav");
	}
}

void JumpState::Update()
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
	Owner->JumpAction();
	Owner->PlayAnimation();
	if (GameInput.GetKey(VK_LEFT) == KEY_HOLD)
	{
		Owner->SetInverse(true);
		if (Owner->GetLeftWallState() == false)
		{
			Owner->PlayerMove(LEFT);
		}
		else
		{
			Owner->ChangeState(new WallClingState());
			return;
		}
	}
	else if (GameInput.GetKey(VK_RIGHT) == KEY_HOLD)
	{
		Owner->SetInverse(false);
		if (Owner->GetRightWallState() == false)
		{
			Owner->PlayerMove(RIGHT);
		}
		else
		{
			Owner->ChangeState(new WallClingState());
			return;
		}
	}
	if (GameInput.GetKey('X') == KEY_PUSH)
	{
		WaitTime = 0.0f;
		Owner->SetAnimation(Owner->FindSprite(L"ShootJump.txt"));
		Owner->ResetChargeState();
		Owner->SpawnBullet();
	}
	else if (GameInput.GetKey('X') == KEY_HOLD)
	{
		Owner->UpdateChargingState();
		if (Owner->GetIsCharge()) {
			Owner->SetAnimation(Owner->FindSprite(L"Jump.txt"));
		}
	}
	else if (GameInput.GetKey('X') == KEY_FREE)
	{
		if (Owner->GetIsCharge())
		{
			Owner->SetAnimation(Owner->FindSprite(L"ShootJump.txt"));
			Owner->SpawnBullet();
			Owner->ResetChargeState();
		}
		else
		{
			WaitTime += gSecondPerFrame;
			if (WaitTime > 0.75f)
			{
				Owner->SetAnimation(Owner->FindSprite(L"Jump.txt"));
			}
		}

	}
	if (GameInput.GetKey('Z') == KEY_PUSH)
	{
		Owner->ChangeState(new AirDashState());
		return;
	}
	if (Owner->CheckJumpAccel())
	{
		Owner->ChangeState(new IdleState());
		return;
	}
}

void JumpState::Exit()
{
	Owner->SetJumpState(false);
	Owner->ResetAnimIndex();
	Owner = nullptr;
}
