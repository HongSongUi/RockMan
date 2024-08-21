#include "WalkState.h"
#include "JumpState.h"
#include "DashState.h"
#include "IdleState.h"
#include "HitState.h"
#include "WinState.h"
#include "Input.h"
#include "Player.h"
void WalkState::Enter()
{
	WaitTime = 0.f;
	Owner->SetAnimation(Owner->FindSprite(L"Walk.txt"));
	Tag = WALK;
}

void WalkState::Update()
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
	if (GameInput.GetKey(VK_LEFT) == KEY_HOLD)
	{
		Owner->SetInverse(true);
		if (Owner->GetLeftWallState() == false)
		{
			Owner->PlayerMove(LEFT);
		}
	}
	else if (GameInput.GetKey(VK_RIGHT) == KEY_HOLD)
	{
		Owner->SetInverse(false);
		if (Owner->GetRightWallState() == false)
		{
			Owner->PlayerMove(RIGHT);
		}
	}
	else if (GameInput.GetKey(VK_LEFT) == KEY_UP || GameInput.GetKey(VK_RIGHT) == KEY_UP)
	{
		Owner->ChangeState(new IdleState());
		return;
	}
	if (GameInput.GetKey('C') == KEY_PUSH)
	{
		Owner->ChangeState(new JumpState());
		return;

	}
	else if (GameInput.GetKey('Z') == KEY_PUSH)
	{
		Owner->ChangeState(new DashState());
		return;

	}
	else if (GameInput.GetKey('X') == KEY_PUSH)
	{
		WaitTime = 0.f;
		Owner->ResetChargeState();
		Owner->SetAnimation(Owner->FindSprite(L"ShootWalk.txt"));
		Owner->SpawnBullet();
	}

	else if (GameInput.GetKey('X') == KEY_HOLD)
	{
		Owner->UpdateChargingState();
		if (Owner->GetIsCharge())
		{
			Owner->SetAnimation(Owner->FindSprite(L"Walk.txt"));
		}
	}
	
	else if (GameInput.GetKey('X') == KEY_FREE)
	{
		if (Owner->GetIsCharge())
		{
			Owner->SetAnimation(Owner->FindSprite(L"ShootWalk.txt"));
			Owner->SpawnBullet();
			Owner->ResetChargeState();
		}
		else
		{
			WaitTime += gSecondPerFrame;
			if (WaitTime > 0.5f)
			{
				Owner->SetAnimation(Owner->FindSprite(L"Walk.txt"));
			}
		}
	}
}

void WalkState::Exit()
{
	Owner->ResetAnimIndex();
}
