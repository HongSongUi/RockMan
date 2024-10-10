#include "IdleState.h"
#include "JumpState.h"
#include "WalkState.h"
#include "CrouchState.h"
#include "ShootState.h"
#include "HitState.h"
#include "DeathState.h"
#include "WinState.h"
#include "Input.h"
#include "Player.h"
void IdleState::Enter()
{
    Owner->SetCurrentState(); // Check Character Health (50%)
    Tag = IDLE;
}

void IdleState::Update()
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

    if (Owner->GetIsGround() == false) 
    {
        Owner->Falling();
    }
    else
    {
        if (GameInput.GetKey('X') == KEY_FREE)
        {
            if (Owner->GetIsCharge())
            {
                Owner->ChangeState(new ShootState());
                return;
            }
        }
        else if (GameInput.GetKey('X') == KEY_HOLD)
        {
            Owner->UpdateChargingState();
        }

        if (GameInput.GetKey(VK_DOWN) == KEY_HOLD)
        {
            Owner->ChangeState(new CrouchState());
            return;
        }
        else if (GameInput.GetKey('C') == KEY_PUSH)
        {
            Owner->ChangeState(new JumpState());
            return;
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
        else if (GameInput.GetKey('X') == KEY_PUSH)
        {
            Owner->ChangeState(new ShootState());
            return;
        }
    }

}

void IdleState::Exit()
{
    Owner->ResetAnimIndex();
    Owner = nullptr;
}
