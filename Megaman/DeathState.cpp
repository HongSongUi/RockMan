#include "DeathState.h"
#include "Player.h"
void DeathState::Enter()
{
	Owner->SetAnimation(Owner->FindSprite(L"HurtA.txt"));
	
}

void DeathState::Update()
{
	Owner->PlayAnimation();
	Owner->ResetAnimIndex();
	WaitTimer += gSecondPerFrame;
	if (WaitTimer >= 1.5f)
	{
		Owner->SetRenderState(false);
	}
}

void DeathState::Exit()
{

}
