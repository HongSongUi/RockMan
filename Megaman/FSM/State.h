#pragma once
enum StateTag
{
	IDLE,
	WALK,
	CROUCH,
	JUMP,
	SHOOT,
	WALLCLING,
	DASH,
	AIRDASH,
	HIT,
};
class State
{
protected:
	StateTag Tag;
	class Player* Owner;
public:
	virtual ~State() {}
	virtual void Enter();
	virtual void Update();
	virtual void Exit();
	virtual void SetOwner(class Player* player);
	virtual StateTag GetTag();
};

