#include "WinState.h"
#include "Player.h"
void WinState::Enter()
{
	Owner->SetCurrentState();
}

void WinState::Update()
{
    if (!Owner->GetStopLoop())
    {
        Owner->PlayAnimation();
    }

    if (!Owner->GetPlayerExit())
    {
        if (Owner->GetIsGround() == false)
        {
            Owner->Falling();
        }
    }
    else
    {
        Owner->SetAnimation(Owner->FindSprite(L"Victory.txt"));
        Owner->SetVitoryEvent();
        if (Owner->IsAnimationEnd())
        {
            Owner->SetAnimationEndLoop();
            Owner->MoveUp();
        }
    }
}

void WinState::Exit()
{
}
