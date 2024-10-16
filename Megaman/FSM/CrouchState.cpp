#include "CrouchState.h"
#include "Player.h"
#include "Input.h"
#include "IdleState.h"
#include "HitState.h"
#include "WinState.h"
void CrouchState::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"Crouch.txt"));
	Tag = CROUCH;
}

void CrouchState::Update()
{
	if (Owner->IsPlayerHit()) // if Player Hit
	{
		if (Owner->IsPlayerCanHit()) // Check Can Player Hit
		{
			Owner->ChangeState(new HitState);
			return;
		}
	}
	if (Owner->GetPlayerWin()) // if Player Win
	{
		Owner->ChangeState(new WinState);
		return;
	}
	Owner->PlayAnimation(); //Play Character Animation
	if (GameInput.GetKey('X') == KEY_PUSH)
	{
		WaitTime = 0.f;
		Owner->SetAnimation(Owner->FindSprite(L"CrouchShoot.txt"));
		Owner->SpawnBullet();
	}
	else if (GameInput.GetKey('X') == KEY_HOLD)
	{
		Owner->UpdateChargingState(); // Charge Start
		if (Owner->GetIsCharge())
		{
			Owner->SetAnimation(Owner->FindSprite(L"Crouch.txt"));
		}
	}
	else if (GameInput.GetKey('X') == KEY_FREE)
	{
		if (Owner->GetIsCharge())
		{
			if (Owner->GetChargeState() == CHARGEND) 
			{
				Owner->SetAnimation(Owner->FindSprite(L"CrouchCharge.txt")); // Crouch Charge Attack Animation
			}
			else
			{
				Owner->SetAnimation(Owner->FindSprite(L"CrouchShoot.txt")); // Crouch Attack Animation
			}
			Owner->SpawnBullet();
			Owner->ResetChargeState();
		}
		else
		{
			WaitTime += gSecondPerFrame;
			if (WaitTime > 0.5f)
			{
				Owner->SetAnimation(Owner->FindSprite(L"Crouch.txt"));
			}
		}

	}

	if (GameInput.GetKey(VK_DOWN) == KEY_FREE)
	{
		Owner->ChangeState(new IdleState());
		return;
	}

}

void CrouchState::Exit()
{
	Owner->ResetAnimIndex();
}
