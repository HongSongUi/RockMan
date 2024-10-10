#include "IntroIdle.h"
#include "Player.h"
#include "IntroWalkState.h"
void IntroIdle::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"Intro.txt")); // Set Player Animation
	Owner->PauseAnimation(0); // Aniation Pause
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
			Owner->PlayEffectSound(L"ROCK_X5_00307.wav"); // Player Drop Sound
			IsPlaySound = true;
		}
		Owner->Falling();
	}
	else
	{
		IsGround = true;
		Owner->ReStartAnimation(); // Play Character Animation
		Owner->SpawnPlayerEvent();
		if (Owner->IsAnimationEnd()) // if is Spawn End
		{
			Owner->ChangeState(new IntroWalkState); 
			return;

		}
	}
}

void IntroIdle::Exit()
{
	Owner->ResetAnimIndex(); // Reset Animation Index to zero
}
