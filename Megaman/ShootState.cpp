#include "ShootState.h"
#include "WalkState.h"
#include "IdleState.h"
#include "JumpState.h"
#include "HitState.h"
#include "WinState.h"
#include "Player.h"
#include "Input.h"
void ShootState::Enter()
{
	if (Owner->GetChargeState() == CHARGEND)
	{
		Owner->SetAnimation(Owner->FindSprite(L"ChargeShoot.txt"));

	}
	else
	{
		Owner->SetAnimation(Owner->FindSprite(L"Shoot.txt"));
	}
	Tag = SHOOT;
	Owner->SpawnBullet();
	Owner->ResetChargeState();
}

void ShootState::Update()
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
	if (GameInput.GetKey('X') == KEY_PUSH)
	{
		Owner->SpawnBullet();
		Owner->ResetChargeState();
		Owner->SetAinmationIndex(0);
	}
	else if (GameInput.GetKey('X') == KEY_HOLD)
	{
		Owner->UpdateChargingState();
		UpdateState();
	}
	else if (GameInput.GetKey('X') == KEY_FREE)
	{
		UpdateState();
	}

	else if (GameInput.GetKey(VK_LEFT) == KEY_HOLD)
	{
		Owner->SetInverse(true);
		Owner->ChangeState(new WalkState());
		return;

	}
	else if (GameInput.GetKey(VK_RIGHT) == KEY_HOLD)
	{
		Owner->SetInverse(false);
		Owner->ChangeState(new WalkState());
		return;

	}

}

void ShootState::Exit()
{
	Owner->ResetAnimIndex();
	Owner = nullptr;
}

void ShootState::UpdateState()
{
	if (GameInput.GetKey(VK_LEFT) == KEY_HOLD)
	{
		Owner->SetInverse(true);
		Owner->ChangeState(new WalkState());
		return;
	}
	else if (GameInput.GetKey(VK_RIGHT) == KEY_HOLD)
	{
		Owner->SetInverse(false);
		Owner->ChangeState(new WalkState());
		return;
	}
	else if (GameInput.GetKey('C') == KEY_HOLD)
	{
		Owner->ChangeState(new JumpState());
		return;

	}
	else if (Owner->CheckAnimationIndex(1))
	{
		Owner->ChangeState(new IdleState());
		return;
	}
}
