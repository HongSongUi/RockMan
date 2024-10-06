#include "IntroIdle.h"
#include "Player.h"
#include "IntroWalkState.h"
void IntroIdle::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"Intro.txt"));
	Owner->PauseAnimation(0);
	IsGround = false;
	IsPlaySound = false;
}

void IntroIdle::Update()
{
	Owner->PlayAnimation();

	if (!Owner->GetIsGround() && IsGround == false)
	{
		if (!IsPlaySound)
		{
			Owner->PlayEffectSound(L"ROCK_X5_00307.wav");
			IsPlaySound = true;
		}
		Owner->Falling();
	}
	else
	{
		IsGround = true;
		Owner->ReStartAnimation();
		Owner->SpawnPlayerEvent();
		if (Owner->IsAnimationEnd())
		{

			Owner->ChangeState(new IntroWalkState);
			return;

		}
	}
}

void IntroIdle::Exit()
{
	Owner->ResetAnimIndex();
}
