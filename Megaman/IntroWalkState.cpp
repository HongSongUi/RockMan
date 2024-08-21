#include "IntroWalkState.h"
#include "IdleState.h"
#include "Player.h"
void IntroWalkState::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"Idle.txt"));
	Owner->SetSpawnState(true);
}

void IntroWalkState::Update()
{
	Owner->PlayAnimation();
	if (!Owner->GetStartIntroMove())
	{
		return;
	}

	if (!Owner->GetIntroMoveEnd())
	{
		Owner->SetAnimation(Owner->FindSprite(L"Walk.txt"));
		Owner->IntroWalk();
	}
	else
	{
		Owner->SetAnimation(Owner->FindSprite(L"Idle.txt"));
	}

	if (Owner->GetIsGameStart())
	{
		Owner->ChangeState(new IdleState);
		return;
	}
}

void IntroWalkState::Exit()
{
	Owner->ResetAnimIndex();
}
