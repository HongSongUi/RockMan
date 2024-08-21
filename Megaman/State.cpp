#include "State.h"
#include "HitState.h"
#include "Player.h"
void State::Enter()
{
}

void State::Update()
{
	
}

void State::Exit()
{
}

void State::SetOwner(Player* player)
{
	Owner = player;
}

StateTag State::GetTag()
{
	return Tag;
}
