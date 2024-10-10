#include "Player.h"
#include "IdleState.h"
#include "../TextLoader.h"
#include "Bullet.h"
#include "Camera2D.h"
#include "Input.h"
#include "../Effect.h"
#include "Boss.h"
#include "SoundManager.h"
#include "IntroIdle.h"
#include "Boss.h"
#include <fstream>
void Player::Falling(float FallingSpeed)
{
	//if Character Jump 
	JumpForce = 0.5f;
	Position.y += 1.0f * gSecondPerFrame * 9.8 * FallingSpeed * JumpForce;
}

bool Player::GetIsGround()
{
	return IsGround;
}

void Player::SetChargeState(ChargeState state)
{
	Charge = state;
}

ChargeState Player::GetChargeState()
{
	return Charge;
}

bool Player::CheckAnimationIndex(int adjust)
{
	if (AnimationIndex >= CurrentAnimation.size() - adjust)
	{
		return true;
	}
	return false;
}

void Player::SetAnimation(std::vector<Rect> Animation)
{
	CurrentAnimation = std::move(Animation);
}

void Player::InitAnimation()
{
	for (int i = 0; i < Loader->fileList.size(); i++) 
	{
		int num = 0;
		std::ifstream file(Loader->fileList[i]);
		std::wstring name = Loader->GetSplitName(Loader->fileList[i]);
		file >> num;
		AnimationList.reserve(num);
		while (file.eof() == false) 
		{
			file >> Sprite.Left >> Sprite.Top >> Sprite.Right >> Sprite.Bottom;
			AnimationList.push_back({ Sprite.Left,Sprite.Top, Sprite.Right,Sprite.Bottom });
		}
		FileList.insert(std::make_pair(name, AnimationList));
		AnimationList.clear();
		file.close();
	}
}

std::vector<Rect> Player::FindSprite(std::wstring name)
{
	auto iter = FileList.find(name);
	if (iter != FileList.end())
	{
		return iter->second;
	}

	return std::vector<Rect>();
}

bool Player::Init()
{
	//Init Player Data
	IsGameStart = false;
	Inverse = false;
	HitCoolTime = 0.75f;
	CanHit = true;
	PlayChargeSound = false;
	IsRender = true;
	PlayerExit = false;
	GameEnd = false;
	StopLoop = false;
	IsPlayDeathSound = true;
	IsPlayExitSound = false;
	JumpForce = 0.5f;
	BulletCoolTime = 0.25f;
	CoolTime = 0.f;
	IsCharge = false;
	PlayVictory = true;
	ChargeTime = 0.f;
	Loader = new TextLoader;
	Loader->LoadDir(L"../_Animation/RockMan/");
	SetMask(L"../_Texture/RockManmask.bmp");
	SetPhysics();
	Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/RockMan.bmp");
	InitAnimation();
	SetRect(FindSprite(L"Idle.txt")[0]);
	CurrentAnimation = FindSprite(L"Idle.txt");
	SetPosition({ 150,-100 });
	ChangeState(new IntroIdle);
	CreateVertex();
	
	InitSound();
	return true;
}
void Player::InitSound()
{
	//Init Player Sound
	ShootSound = new GameSound;
	ChargingSound = new GameSound;
	PlayerSound = new GameSound;
	ChargeShootSound = new GameSound;
	DeathSound = new GameSound;
	BulletHitSound = new GameSound;
	VictorySound = new GameSound;
	ExitSound = new GameSound;

	ShootSound = SoundMgr.Find(L"ROCK_X5_00744.wav");
	ChargingSound = SoundMgr.Find(L"ROCK_X5_00183.wav");
	ChargeShootSound = SoundMgr.Find(L"ROCK_X5_00328.wav");
	DeathSound = SoundMgr.Find(L"ROCK_X5_00213.wav");
	BulletHitSound = SoundMgr.Find(L"ROCK_X5_00427.wav");
	VictorySound = SoundMgr.Find(L"ROCK_X5_00290.wav");
	ExitSound = SoundMgr.Find(L"ROCK_X5_00309.wav");
}

bool Player::IntroFrame()
{
	//character intro frame
	PlayAnimation();
	SetPosition(Position);
	UpdateVertexList();
	return true;
}

bool Player::Frame()
{
	// character main frame
	HealthScale = (MaxHealth - CurHealth) / 100;
	CoolTime -= gSecondPerFrame;

	Position = WorldPos;
	CurrentState->Update();// fsm
	
	SetPosition(Position);
	

	UpdateBullet(CameraPosition, CameraSize);
	CheckHitCoolTime();
	PlayBulletHitEffect();
	return true;
}

void Player::IntroFalling()
{
	// play character intro landing
	MoveEnd = false;
	if (Intro == true)
	{
		std::vector<Rect> find_sprite = FindSprite(L"Intro.txt");
		SetAnimation(find_sprite);
	}
	if (IsGround == false && HitGround == false) {
		EffectSound->Play();
		JumpForce = 0.5f;
		Position.y += 1.0f * gSecondPerFrame * 15.0f * Y_Speed * JumpForce; // falling to ground
		AnimationIndex = 0;
	}
	else if (IsGround == true || HitGround == true)
	{
		HitGround = true;
		if (AnimationIndex >= CurrentAnimation.size() - 5)
		{
			if (Intro == true) {
				EffectSound = SoundMgr.Find(L"ROCK_X5_00314.wav"); // play character spawn sound
				EffectSound->Play();
			}
		}
		if (AnimationIndex >= CurrentAnimation.size() - 1) // handle character spawn end
		{
			if (Intro == true) 
			{
				IntroFinish = true;
			}
			else 
			{
				IntroFinish = false;
			}
			Intro = false;
			std::vector<Rect> find_sprite = FindSprite(L"Intro.txt");
			SetAnimation(find_sprite);
		}
	}
}



void Player::IntroWalk()
{
	if (Position.x <= 500) 
	{
		PlayerMove(RIGHT);
	}
	else
	{
		MoveEnd = true;
	}
}

void Player::SetSpawnState(bool state)
{
	IsSpawn = state;
}

bool Player::GetSpawnState()
{
	return IsSpawn;
}
void Player::SetStartIntroMove(bool state)
{
	MoveStart = true;
}

bool Player::GetStartIntroMove()
{
	return MoveStart;
}

bool Player::GetIsGameStart()
{
	return IsGameStart;
}

float Player::GetHealthScale()
{
	return HealthScale;
}

void Player::SetGameStart(bool is_start)
{
	IsGameStart = is_start;
}

bool Player::GetIntroMoveEnd()
{
	return MoveEnd;
}

void Player::PlayEffectSound(std::wstring file_name)
{
	EffectSound = SoundMgr.Find(file_name);
	EffectSound->Play();
}

void Player::SpawnPlayerEvent()
{
	if (AnimationIndex >= CurrentAnimation.size() - 5)
	{
		PlayEffectSound(L"ROCK_X5_00314.wav");
	}

}

void Player::PauseAnimation(int idx)
{
	AnimationIndex = idx;
	IsPauseAnimation = true;
}
void Player::ReStartAnimation()
{
	IsPauseAnimation = false;
}

bool Player::Render()
{
	//player render
	if (IsRender == false)
	{
		return true;
	}
	MaskRender();
	BulletRender();
	if (IsCharge == true)
	{
		ChargeEffect->MaskRender(); // if charge state play charge effect
	}
	//play effect
	if (CurrentTag == DASH || CurrentTag == AIRDASH) // play dash , air dash effect
	{
		if (CurrentTag == DASH)
		{
			if (DashDustEffect->CheckAnimationEnd() == false)
			{
				DashDustEffect->MaskRender();
			}
		}
		if (DashBoostEffect->CheckAnimationEnd() == false)
		{
			DashBoostEffect->MaskRender();
		}		
	}
	else if (CurrentTag == WALLCLING)
	{
		WallDustEffect->MaskRender();
	}

	if (IsPlayBulletHitEffect)
	{
		BulletHitEffect->MaskRender();
	}
	return true;
}

void Player::SetCurrentState()
{
	//check character health
	if (CurHealth <= MaxHealth / 2.0f) 
	{
		std::vector<Rect> find_sprite = FindSprite(L"IdleLowHp.txt");
		SetAnimation(find_sprite);
	}
	else 
	{
		std::vector<Rect> find_sprite = FindSprite(L"Idle.txt");
		SetAnimation(find_sprite);
	}
}

void Player::ChangeState(State* NewState)
{
	if (CurrentState)
	{
		PrevTag = CurrentState->GetTag();
		CurrentState->Exit();
		delete CurrentState;
		CurrentState = nullptr;
	}
	CurrentState = NewState;
	CurrentState->SetOwner(this);
	CurrentState->Enter();
	CurrentTag = CurrentState->GetTag();
}

void Player::PlayAnimation()
{

	if (AnimationIndex >= CurrentAnimation.size())
	{
		AnimationIndex = 0;
	}
	SetRect(CurrentAnimation[AnimationIndex]);
	float LifeTime = CurrentAnimation.size() * 0.1;
	float frame = 0.0f;
	Time += gSecondPerFrame;
	frame = LifeTime / CurrentAnimation.size();
	if (Time >= frame)
	{
		if (IsPauseAnimation == false)
		{
			AnimationIndex++;
		}
		HandleAnimationLoop();
		Time = Time - frame;
	}
}
void Player::HandleAnimationLoop()
{
	if (AnimationIndex >= CurrentAnimation.size())
	{
		if (AnimLoop)
		{
			AnimationIndex = 2;
		}
		if (AnimEndLoop)
		{
			AnimationIndex = CurrentAnimation.size() - 1;
		}
		else
		{
			AnimationIndex = 1;
		}
	}
}

void Player::SetAinmationIndex(int idx)
{
	AnimationIndex = idx;
}

bool Player::Release()
{
	Object2D::Release();
	BulletRelease();

	ChargeEffect->Release();
	DashDustEffect->Release();
	DashBoostEffect->Release();
	WallDustEffect->Release();
	BulletHitEffect->Release();
	return true;
}

bool Player::CheckJumpAccel()
{
	if (JumpForce <0.f)
	{
		if (IsGround == true) 
		{
			AnimEndLoop = false;
			SetPlaySound(L"ROCK_X5_00070.wav");
			return true;
		}
		return false;
	}
	return false;
}

void Player::ResetAnimIndex()
{
	AnimationIndex = 0;
}

void Player::PlayerMove(int Dir)
{
	if (Dir == LEFT)
	{
		Position.x += -1.0f * gSecondPerFrame * Speed;
	}
	else if (Dir == RIGHT) 
	{
		Position.x += 1.0f * gSecondPerFrame * Speed;
	}
}
bool Player::SetData(ID3D11Device* device, ID3D11DeviceContext* context, RECT clientRt)
{
	D3D11Device = device;
	D3D11Context = context;
	Mesh.SetDevice(device, context);
	ClientRect = clientRt;

	EffectInit();
	return true;
}
void Player::EffectInit()
{
	ChargeEffect = new Effect;
	ChargeEffect->SetData(D3D11Device, D3D11Context, ClientRect);
	ChargeEffect->SetTexture(L"../_Texture/Buster.bmp", L"../_Texture/Bustermask.bmp");
	ChargeEffect->Tag = EffectTag::CHRGING;
	ChargeEffect->Init();

	DashDustEffect = new Effect;
	DashDustEffect->SetData(D3D11Device, D3D11Context, ClientRect);
	DashDustEffect->SetTexture(L"../_Texture/Dash.bmp", L"../_Texture/Dashmask.bmp");
	DashDustEffect->Init();

	
	DashBoostEffect = new Effect;
	DashBoostEffect->SetData(D3D11Device, D3D11Context, ClientRect);
	DashBoostEffect->SetTexture(L"../_Texture/DashBooster.gif", L"../_Texture/DashBoostermask.bmp");
	DashBoostEffect->Init();

	WallDustEffect = new Effect;
	WallDustEffect->SetData(D3D11Device, D3D11Context, ClientRect);
	WallDustEffect->SetTexture(L"../_Texture/WallEffect.bmp", L"../_Texture/WallEffectmask.bmp");
	WallDustEffect->Init();
	WallDustEffect->SetLoop(true);

	BulletHitEffect = new Effect;
	BulletHitEffect->SetData(D3D11Device, D3D11Context, ClientRect);
	BulletHitEffect->SetTexture(L"../_Texture/Buster2.bmp", L"../_Texture/Buster2mask.bmp");
	BulletHitEffect->Init();
}

void Player::PlayChargeEffect()
{
	ChargeEffect->SetPosition(WorldPos);
	std::vector<Rect> find_sprite = ChargeEffect->FindSprite(L"Charging.txt");
	ChargeEffect->SetSprite(find_sprite);
	ChargeEffect->Frame();
	ChargeEffect->CameraSet(CameraPosition, CameraSize);
	if (PlayChargeSound == false)
	{
		ChargingSound->Play();
		PlayChargeSound = true;
	}
}

void Player::PlayDashEffect()
{
	DashDustEffect->SetInverse(!Inverse);
	DashBoostEffect->SetInverse(!Inverse);
	if (Inverse) // Set dash effect position use character inverse state
	{
		DashDustEffect->SetPosition({ ObjectRect.Max.x + 55, ObjectRect.Max.y });
		DashBoostEffect->SetPosition({ ObjectRect.Max.x + 55, ObjectRect.Max.y });
	}
	else
	{
		DashDustEffect->SetPosition({ ObjectRect.Min.x - 55, ObjectRect.Max.y });
		DashBoostEffect->SetPosition({ ObjectRect.Min.x - 55, ObjectRect.Max.y });
	}
	DashDustEffect->SetSprite(DashDustEffect->FindSprite(L"DashEffect.txt"));
	DashDustEffect->Frame();
	DashDustEffect->CameraSet(CameraPosition, CameraSize);
	
	DashBoostEffect->SetSprite(DashBoostEffect->FindSprite(L"DashBooster.txt"));
	DashBoostEffect->Frame();
	DashBoostEffect->CameraSet(CameraPosition, CameraSize);
}

void Player::DashEndEvent()
{
	DashDustEffect->ResetEffect();
	DashBoostEffect->ResetEffect();
}

void Player::PlayWallEffect()
{
	// set wall dust effect position, inverse state
	if (LeftWall) 
	{
		WallDustEffect->SetInverse(false);
		WallDustEffect->SetPosition({ ObjectRect.Min.x, ObjectRect.Max.y });
	}
	else if (RightWall)
	{
		WallDustEffect->SetInverse(true);
		WallDustEffect->SetPosition({ObjectRect.Max.x, ObjectRect.Max.y });
	}
	WallDustEffect->SetSprite(WallDustEffect->FindSprite(L"WallClingEffect.txt"));
	WallDustEffect->Frame();
	WallDustEffect->CameraSet(CameraPosition, CameraSize);

}

void Player::PlayBulletHitEffect()
{
	//player bullet hit effect
	if (IsPlayBulletHitEffect)
	{
		BulletHitEffect->Frame();
		BulletHitEffect->CameraSet(CameraPosition, CameraSize);
		if (BulletHitEffect->CheckAnimationEnd())
		{
			IsPlayBulletHitEffect = false;
			BulletHitEffect->ResetAnimation();
		}
	}
}

void Player::WallClingEndEvent()
{
	WallDustEffect->ResetEffect();
}

void Player::HitEndEvent()
{
	IsHit = false;
}

Vector2 Player::GetPosition()
{
	return WorldPos;
}

Rect Player::GetObjectRect()
{
	return ObjectRect;
}

void Player::GetDamage(float damage)
{
	if (CanHit == false)
	{
		return;
	}
	CurHealth -= damage;
	if (CurHealth <= 0.f)
	{
		PlayerDeath = true;
	}
	IsHit = true;
}
bool Player::IsPlayerDeath()
{
	return PlayerDeath;
}

StateTag Player::GetPrevState()
{
	return PrevTag;
}

bool Player::IsPlayerHit()
{
	return IsHit;
}

void Player::SetRenderState(bool state)
{
	if (IsPlayDeathSound)
	{
		DeathSound->Play();
		IsPlayDeathSound = false;
	}
	IsRender = state;
}

void Player::SetPlayerWin(bool state)
{
	PlayerWin = state;
}

bool Player::GetPlayerWin()
{
	return PlayerWin;
}

void Player::SetAnimationEndLoop()
{
	AnimEndLoop = true;
}

void Player::MoveUp()
{
	//use outro (player win)
	if (!IsPlayExitSound)
	{
		ExitSound->Play();
		IsPlayExitSound = true;
	}
	Position.y += -1.0f * gSecondPerFrame * 9.8 * Y_Speed * JumpForce;
	if (Position.y < -100.f)
	{
		GameEnd = true;
	}
}

bool Player::GetGameEnd()
{
	return GameEnd;
}

void Player::SetVitoryEvent()
{
	if (AnimationIndex == 4)
	{
		if (PlayVictory == true) 
		{
			VictorySound->PlayEffect(0.5f);// play victory sound
			PlayVictory = false;
		}
		StopLoop = true;
		VictoryTimer -= gSecondPerFrame;
		if (VictoryTimer < 0.0f) 
		{
			StopLoop = false;
		}
	}
}

bool Player::GetStopLoop()
{
	return StopLoop;
}

void Player::ResetEndLoop()
{
	AnimEndLoop = false;
}


void Player::SetPlayerExit(bool state)
{
	PlayerExit = state;
}

bool Player::GetPlayerExit()
{
	return PlayerExit;
}

void Player::SetHitStartEvent()
{
	CanHit = false;
}

bool Player::CheckBulletCollision(Object2D* object)
{
	for (auto iter = BulletList.begin(); iter != BulletList.end(); ) // loop bullet use bullet list
	{
		Bullet* bul = *iter;
		Rect obj_rect = object->GetObjectRect();
		if (Collision::RectToRect(bul->ObjectRect, obj_rect)) // bullet rect / object rect collision check
		{
			SpawnBulletHitEffect(bul, object);
			bul->Release();
			delete bul;
			iter = BulletList.erase(iter);
			bul = nullptr;

			continue;
			return true;
		}
		iter++;
	}
	return false;
}

void Player::SpawnBulletHitEffect(Bullet* bullet , Object2D* object)
{
	Boss* boss = dynamic_cast<Boss*>(object);
	BulletHitSound->Play();
	if (boss == nullptr)
	{
		return;
	}
	BulletHitEffect->SetPosition(boss->GetWorldPosition());
	BulletHitEffect->SetInverse(boss->GetInverse());
	std::vector<Rect> hit_effect;
	if (bullet->GetType() == SEC_CHARGE)
	{
		IsPlayBulletHitEffect = true;
		BulletHitEffect->SetSprite(BulletHitEffect->FindSprite(L"SecondHit.txt"));
	}
	else if (bullet->GetType() == FULL_CHARGE)
	{
		IsPlayBulletHitEffect = true;
		if (boss->GetBossState() != REFLECT)
		{
			BulletHitEffect->SetSprite(BulletHitEffect->FindSprite(L"ChargeHit.txt"));
		}
		else
		{
			BulletHitEffect->SetSprite(BulletHitEffect->FindSprite(L"GuardHit.txt"));
		}
	}

	if (boss->GetBossState() != REFLECT)
	{
		boss->GetDamage(bullet->GetBulletDamage());
	}
}

void Player::SetPlaySound(std::wstring file_name)
{
	PlayerSound = SoundMgr.Find(file_name);
	PlayerSound->PlayEffect(0.5f);
}

bool Player::IsPlayerCanHit()
{
	return CanHit;
}

void Player::CheckHitCoolTime()
{
	//if character hit, don't hit during hit cool time
	if (CanHit == false)
	{
		HitCoolTime -= gSecondPerFrame;
		if (HitCoolTime <= 0.f)
		{
			CanHit = true;
			HitCoolTime = 0.75f;
		}
	}
}


void Player::SetCameraData(Vector2 Campos,Vector2 CamSize )
{
	CameraPosition = Campos;
	CameraSize = CamSize;
}

void Player::PlayerDash(float PlayerSpeed)
{
	if (Inverse == true)
	{
		Position.x += -1.0f * gSecondPerFrame * PlayerSpeed;
	}
	else if (Inverse == false)
	{
		Position.x += 1.0f * gSecondPerFrame * PlayerSpeed;
	}

}

bool Player::GetInverseState()
{
	return Inverse;
}

void Player::SetLeftWallState(bool state)
{
	LeftWall = state;
}

void Player::SetRightWallState(bool state)
{
	RightWall = state;
}

void Player::SetGroundState(bool state)
{
	IsGround = state;
}


void Player::SpawnBullet()
{
	if (CoolTime <= 0.f)
	{
		InitBullet();
		CoolTime = BulletCoolTime;
		ShootSound->PlayEffect(0.5f);
	}
}

void Player::SetFallingAnim()
{
	SetAinmationIndex(FindSprite(L"Jump.txt").size()-1);
}

bool Player::GetRightWallState()
{
	return RightWall;
}

void Player::InitBullet()
{
	// Spawn character bullet
	PlayerBullet = new Bullet;
	PlayerBullet->SetData(D3D11Device, D3D11Context, ClientRect);
	PlayerBullet->Init();
	if (Charge == NOCHARGE)
	{
		PlayerBullet->SetType(NORMAL);
	}
	else if (Charge == CHARGING)
	{
		ChargeShootSound = SoundMgr.Find(L"ROCK_X5_00304.wav");
		ChargeShootSound->PlayEffect(0.5f);
		PlayerBullet->SetType(SEC_CHARGE);
	}
	else if (Charge == CHARGEND)
	{
		ChargeShootSound = SoundMgr.Find(L"ROCK_X5_00328.wav");
		ChargeShootSound->PlayEffect(0.5f);
		PlayerBullet->SetType(FULL_CHARGE);
	}
	SetBulletSpawn();
	SetBulletSpawnPoint(PlayerBullet);

	BulletList.push_back(PlayerBullet);
}

void Player::SetBulletSpawn()
{
	//Set bullet Inverse
	if (CurrentTag == WALLCLING)
	{
		PlayerBullet->SetInverse(Inverse);
	}
	else
	{
		PlayerBullet->SetInverse(!Inverse);
	}
}

void Player::SetBulletSpawnPoint(Bullet* bullet)
{
	// Handle Bullet Spawn point
	if (Inverse) 
	{
		if (CurrentTag == WALLCLING) 
		{
			BulletSpawnPoint.x = ObjectRect.Max.x + 15;
		}
		else
		{
			BulletSpawnPoint.x = ObjectRect.Min.x - 15;
		}
	}
	else 
	{
		if (CurrentTag == WALLCLING) 
		{
			BulletSpawnPoint.x = ObjectRect.Min.x - 15;
		}
		else 
		{
			BulletSpawnPoint.x = ObjectRect.Max.x + 15;
		}
	}
	if (CurrentTag == WALLCLING || CurrentTag == CROUCH)
	{
		BulletSpawnPoint.y = ObjectRect.Min.y + (ObjectRect.Size.y / 3.0f);
	}
	else 
	{
		BulletSpawnPoint.y = ObjectRect.Min.y + (ObjectRect.Size.y / 3.0f) - 10;
	}
	bullet->SetSpawnPosition(BulletSpawnPoint);
}

void Player::UpdateBullet(Vector2 CamPos, Vector2 CamSize)
{
	for (auto iter = BulletList.begin(); iter != BulletList.end(); )
	{
		auto bullet = *iter;
		bullet->Frame();
		bullet->CameraSet(CamPos, CamSize);
		if (bullet->ReadyToFire())
		{
			SetBulletSpawnPoint(bullet);
		}
		if (bullet->CheckBulletTime())
		{
			bullet->Release();
			delete bullet;
			bullet = nullptr;
			iter = BulletList.erase(iter);
			continue;
		}
		iter++;
	}
}
void Player::BulletRender()
{
	for (auto iter = BulletList.begin(); iter != BulletList.end(); iter++)
	{
		auto bullet = *iter;
		bullet->MaskRender();
	}
}

void Player::BulletRelease()
{
	for (auto iter = BulletList.begin(); iter != BulletList.end(); iter++)
	{
		auto bullet = *iter;
		bullet->Release();
	}
}

void Player::UpdateChargingState()
{
	ChargeTime += gSecondPerFrame;

	if (ChargeTime >= 0.5f&& ChargeTime<1.0f)
	{
		PlayChargeEffect();
		
		IsCharge = true;
		Charge = CHARGING;
	}
	else if (ChargeTime >= 1.0f)
	{
		PlayChargeEffect();
		IsCharge = true;
		Charge = CHARGEND;
	}
}

void Player::ResetChargeState()
{
	ChargeTime = 0.f;
	Charge = NOCHARGE;
	IsCharge = false;
	ChargeStart = false;
	ChargeEffect->ResetEffect();
	ChargingSound->Stop();
	PlayChargeSound = false;
}

float Player::GetChargeTime()
{
	return ChargeTime;
}

bool Player::GetIsCharge()
{
	return IsCharge;
}

void Player::SetInverse(bool state)
{
	Inverse = state;
}


void Player::SetJumpState(bool state)
{
	AnimEndLoop = state;
	IsJump = state;
}

void Player::JumpAction()
{
	Position.y += -1.0f * gSecondPerFrame * 9.8 * Y_Speed * JumpForce;
	JumpForce -= gSecondPerFrame;
}

void Player::UpdateJumpSpeed(float Force)
{
	if (PrevTag != AIRDASH && PrevTag != HIT)
	{
		JumpForce = Force;
	}
	else
	{
		JumpForce = 0.f;
	}
}

bool Player::GetLeftWallState()
{
	return LeftWall;
}

bool Player::IsAnimationEnd()
{
	return AnimationIndex >= CurrentAnimation.size() - 1;
}