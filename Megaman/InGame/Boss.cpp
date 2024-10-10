#include "Boss.h"
#include "../TextLoader.h"
#include "BossBullet.h"
#include "Laser.h"
#include "SoundManager.h"
#include "../Effect.h"
#include <random>
#include <fstream>

bool Boss::Init()
{
	// Init Boss Data

	Speed = 300.f;
	Loader = new TextLoader;
	EffectInit();
	Loader->LoadDir(L"../_Animation/Dynamo/");
	SetMask(L"../_Texture/Dynamomask.bmp");
	Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/Dynamo.bmp");
	SetAnimation();
	SetRect(FindSprite(L"Idle.txt")[0]);
	Play = FindSprite(L"Idle.txt");
	SetPosition({ 700,-100 });
	CreateVertex();
	EffectSound = SoundMgr.Find(L"ROCK_X5_00307.wav");
	SoundInit();
	return true;
}

void Boss::SoundInit()
{
	EffectSound = new GameSound;
	Voice = new GameSound;
	GigaCharge = new GameSound;
	ExplosionSound = new GameSound;
	LaserSound = new GameSound;

	EffectSound = SoundMgr.Find(L"ROCK_X5_00307.wav");
	GigaCharge = SoundMgr.Find(L"ROCK_X5_00428.wav");
	ExplosionSound = SoundMgr.Find(L"ROCK_X5_00592.wav");
	LaserSound = SoundMgr.Find(L"ROCK_X5_00191.wav");
}

void Boss::SetBossState(Vector2 pos)
{
	// Set Boss Action use Random value
	if (IsDeath == false)
	{
		SetPlayerPos(pos);
		if (StateChange == true)
		{
			if (Wait == true)
			{
				RandomState = READY;
				WaitTime -= gSecondPerFrame; // Standby for Next Action
				if (WaitTime < 0.0f) {
					Wait = false;
					WaitTime = 1.0f;
				}
			}
			else if (Wait == false) {
				StateChange = false;
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<int> dis(0, 3);
				RandomState = (BossState)dis(gen); // Set Boss Action
				Wait = true;
				AnimIndex = 0;
				StateReady();
			}
		}
	}
	else if (IsDeath == true)
	{
		RandomState = DIE;
	}
}

void Boss::IntroFrame()
{
	// Boss Intro Action
	vPos = WorldPos;

	if (Intro == true)
	{
		SetSprite(FindSprite(L"Intro.txt")); // Set Boss Sprite
		if (Ground == false && HitGround == false) {
			JumpAccel = 0.5f;
			vPos.y += 1.0f * gSecondPerFrame * 15.0f * FallingSpeed * JumpAccel;
			AnimIndex = 0;
			EffectSound->Play();
		}
		else if (Ground == true || HitGround == true) // if Hit Ground == true
		{
			HitGround = true;
			if (AnimIndex >= Play.size() - 1)
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
				SetSprite(FindSprite(L"Idle.txt"));
			}
		}
	}
	if (AnimIndex >= Play.size() - 1)
	{
		IntroFinish = true;
	}
	PlayAnimation();
	SetPosition(vPos);
}

bool Boss::GetIntroEnd()
{
	return IntroFinish;
}

float Boss::GetHealthScale()
{
	return HealthScale;
}

void Boss::EffectInit()
{
	// Boss Effect Init
	for (int idx = 0; idx < 2; idx++)
	{
		Effect* Explosion = new Effect;
		Explosion->SetData(D3D11Device, D3D11Context, ClientRect);
		Explosion->SetTexture(L"../_Texture/Explosion.bmp", L"../_Texture/Explosionmask.bmp");
		Explosion->Init();
		ExplosionList.emplace_back(Explosion); 
	}
}

void Boss::StartDeathEvent()
{
	DeathEvent = true;
	IsBossDeath = true;
}

void Boss::PlayExplosionEffect()
{
	// when Boss Death Play Explosion Effect
	if (DeathEvent == false)
	{
		return;
	}

	if (ExplosionStart && !Routine)
	{
		StartExplosion();
	}
	if (CurrnetExplosionTime < 7.f && Routine)
	{
		UpdateExplosionAnimation();
	}
	if (CurrnetExplosionTime > 5.f)
	{
		IsEndEffect = true;
	}
}

void Boss::EffectRender()
{
	if (DeathEvent == false)
	{
		return;
	}
	if (IsEndEffect == true)
	{
		return;
	}
	for (int i = 0; i < 2; i++)
	{
		ExplosionList[i]->MaskRender();
	}
}

void Boss::StartExplosion()
{
	ExplosionSound->PlayEffect(0.25f);
	Explosion = true;// Set explosion flag to true

	for (int i = 0; i < 2; i++) // Set position, sprite, and camera for each explosion effect
	{
		ExplosionList[i]->SetPosition(WorldPos);
		ExplosionList[i]->SetSprite(ExplosionList[i]->FindSprite(L"Explosion.txt"));
		ExplosionList[i]->Frame();
		ExplosionList[i]->CameraSet(CameraPosition, CameraSize);
	}
	Routine = true;
	ExplosionStart = false;
}

void Boss::UpdateExplosionFrame(Effect* explosion)
{
	if (explosion->CheckAnimationEnd())// If explosion animation has ended, play sound, reset animation, and set a new random position
	{
		ExplosionSound->PlayEffect(0.25f);
		explosion->ResetAnimation();
		explosion->SetPosition({ WorldPos.x + randstep(-100, 100), WorldPos.y + randstep(-100, 100) });
	}
	explosion->Frame();
	explosion->CameraSet(CameraPosition, CameraSize);
}

bool Boss::GetBossDeath()
{
	return IsBossDeath;
}

void Boss::ExplosionRelease()
{
	for (auto iter : ExplosionList)
	{
		Effect* effect = iter;
		effect->Release();
		delete effect;
		effect = nullptr;
	}
}

bool Boss::GetIsEffectEnd()
{
	return IsEndEffect;
}

bool Boss::GetIsEnding()
{
	return Ending;
}

Vector2 Boss::GetWorldPosition()
{
	return WorldPos;
}

bool Boss::GetInverse()
{
	return Inverse;
}

BossState Boss::GetBossState()
{
	return RandomState;
}

void Boss::StateReady()
{
	//Adjusting the value to status
	if (RandomState == SWORD)
	{
		Dir.x = PlayerPos.x - vPos.x;
		Dir.y = PlayerPos.y - vPos.y;
		Dir.Normalize();
	}
	else if (RandomState == REFLECT)
	{
		Inverse = !Inverse;
	}
	else if (RandomState == GIGA)
	{
		GigaCharge->PlayEffect(0.5f);
	}
}

float Boss::GetBossDamage()
{
	return Damage;
}

float Boss::GetBulletDamage()
{
	return Bullet->GetBulletDamage();
}


void Boss::UpdateExplosionAnimation()
{
	CurrnetExplosionTime += gSecondPerFrame;

	UpdateExplosionFrame(ExplosionList[0]);// Update the frame for the first explosion effect

	ExplosionWait -= gSecondPerFrame;
	if (ExplosionWait < 0.0f && TimeWait) // Countdown for the next explosion, trigger the second explosion when ready
	{
		NextExplosion = true;
		UpdateExplosionFrame(ExplosionList[1]);
		TimeWait = false;


		ExplosionWait = ExplosionTime - gSecondPerFrame; // Reset the explosion wait time
	}

	if (NextExplosion && !TimeWait)// If the second explosion is triggered and waiting, update its frame and reset it
	{
		UpdateExplosionFrame(ExplosionList[1]);
		if (ExplosionList[1]->CheckAnimationEnd())
		{
			ExplosionList[1]->ResetAnimation();
			NextExplosion = false;
			TimeWait = true;
		}
	}
}

bool Boss::Frame()
{
	if (LoopStop == false)
	{
		PlayAnimation();
	}

	if (Bullet != nullptr)
	{
		Bullet->Frame();
		Bullet->CameraSet(CameraPosition, CameraSize);
	}

	PlayExplosionEffect();
	LaserFrame();

	SetPosition(vPos);
	return true;
}

void Boss::OutroFrame()
{
	// Handle the boss's outro when it loses
	vPos = WorldPos;
	OutroWait -= gSecondPerFrame;// Decrease the wait timer for the outro
	if (OutroWait > 0.5f) 
	{
		Play = FindSprite(L"LowHp.txt");// Play low health sprite
	}
	else if (OutroWait < 0.5f) 
	{
		if (Return == false) 
		{
			// Play voice effect and change sprite on return
			Voice = SoundMgr.Find(L"ROCK_X5_00484.wav");
			Voice->PlayEffect(0.25f);
			Play = FindSprite(L"Return.txt");
			Return = true; // Set return flag to true
			EndLoop = true;
		}
	}
	if (Return == true) 
	{
		if (AnimIndex > 10) 
		{
			vPos.y += -1.0f * gSecondPerFrame * 15.0f * FallingSpeed * JumpAccel;
			if (!IsPlayExitSound)
			{
				EffectSound = SoundMgr.Find(L"ROCK_X5_00307.wav");	// Play exit sound effect when the boss starts falling
				EffectSound->Play();
				IsPlayExitSound = true;// Check exit sound has been played
			}
		}

	}
	if (vPos.y < -100.0f)
	{
		Ending = true;
	}
	PlayAnimation();
	SetPosition(vPos);
}

bool Boss::Render()
{
	MaskRender();
	if (Bullet != nullptr)
	{
		Bullet->MaskRender();
	}
	LaserRender();
	EffectRender();
	return true;
}

bool Boss::Release()
{
	Object2D::Release();
	if (Bullet != nullptr)
	{
		Bullet->Release();
	}
	LaserRelease();
	ExplosionRelease();
	return true;
}

void Boss::Update()
{
	//Update Boss Action use Switch case
	Falling();
	switch (RandomState)
	{
	case READY:
		Damage = 10.f;
		SetBossInverse();
		SetSprite(FindSprite(L"Idle.txt"));
		break;
	case SWORD:
		Damage = 15.f;
		EffectSound = SoundMgr.Find(L"ROCK_X5_00472.wav");
		SetSprite(FindSprite(L"Sword.txt")); // Set Boss Sprite
		if (AnimIndex == 0)
		{
			Voice = SoundMgr.Find(L"ROCK_X5_00475.wav"); // Play Boss Sound
			Voice->PlayEffect(0.025f);
		}
		if (AnimIndex == 8)
		{
			EffectSound->PlayEffect(0.025f); // Play Boss Sword Effect Sound
		}

		if (!LeftWall && !RightWall) // Check Left/ Right Wall Collision
		{
			vPos.x = vPos.x + Dir.x * gSecondPerFrame * Speed;
		}
		vPos.y += -1.0f * gSecondPerFrame * FallingSpeed * JumpAccel;
		break;
	case GIGA:
		Damage = 10.f;
		Voice = SoundMgr.Find(L"ROCK_X5_00498.wav");
		SetSprite(FindSprite(L"Giga.txt"));
		if (AnimIndex == Play.size() - 4)
		{
			Voice->PlayEffect(0.05f);
		}
		if (AnimIndex >= Play.size())
		{
			GigaAttack = true;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dis(0, 3);// Boss has 3 Giga Pattern 
			GigaPattern = dis(gen); // Random Giga Pattern
			if (GigaPattern == 0 || GigaPattern == 1)
			{
				LaserCount = 3; //Number of lasers to create
			}
			else
			{
				LaserCount = 8; //Number of lasers to create
			}
			SpawnLaser(); //Create Laser
		}
		else if (GigaAttack == true)
		{
			SetSprite(FindSprite(L"Idle.txt"));
			StateChange = false;
			LaserTime -= gSecondPerFrame;
			if (LaserTime < 0.0f)
			{
				StateChange = true;
				LaserTime = 0.25f;
				GigaAttack = false;
			}
		}
		break;
	case BOOMERANG:
		Damage = 10.f;
		SetSprite(FindSprite(L"boomerang.txt"));
		//Handle Event use Animation Index
		if (AnimIndex == 0)
		{
			Voice = SoundMgr.Find(L"ROCK_X5_00453.wav");
			Voice->PlayEffect(0.05f);
		}
		else if (AnimIndex == 9)
		{
			CreateBossBullet();
		}
		if (GetBoomerang == false)
		{
			if (AnimIndex == 11)
			{
				LoopStop = true; // Loop Last Frame of Animation 
			}
		}
		else if (GetBoomerang == true)
		{
			LoopStop = false;
		}
		break;
	case REFLECT:
		Damage = 10.f;
		GuardTime -= gSecondPerFrame;
		SetSprite(FindSprite(L"Reflect.txt"));
		if (AnimIndex == 0)
		{
			Voice = SoundMgr.Find(L"ROCK_X5_00457.wav");
			Voice->PlayEffect(0.025f);
		}
		Loop = true;
		if (GuardTime < 0.0f) // Check Guard Time
		{
			GuardTime = 2.0f;
			Loop = false;
			StateChange = true;
		}
		break;
	case DIE:
		SetSprite(FindSprite(L"Die.txt"));
		StartDeathEvent();
		if (IsDeath == false)
		{
			RandomState = LOWHPIDEL;
		}
		break;
	case LOWHPIDEL:
		SetSprite(FindSprite(L"LowHp.txt"));
		break;
	}
}
void Boss::UpdateState(Vector2 pos)
{
	HealthScale = (BossHp - CurrBossHp) / 100; // Calc Health Scale for Health Bar UI
	CurrIdle = FindSprite(L"Idle.txt");
	vPos = WorldPos;
	if (CurrBossHp <= 0)
	{
		IsDeath = true;
	}
	SetBossState(pos);
	Update();
	Frame();
}


void Boss::SetPosition(Vector2 pos)
{
	WorldPos = pos;
	if (RandomState == SWORD)
	{
		if (AnimIndex == 8)
		{
			Size.x = TextureRt.Size.x + 100;
			Size.y = TextureRt.Size.y + 30;
		}
		else
		{
			Size.x = TextureRt.Size.x + 30;
			Size.y = TextureRt.Size.y + 30;
		}
	}
	else
	{
		Size.x = TextureRt.Size.x + 30;
		Size.y = TextureRt.Size.y + 30;
	}

	ObjectRect.SetRect(WorldPos, Size);
}

void Boss::Falling()
{
	if (Ground == false)
	{
		if (RandomState != SWORD)
		{
			JumpAccel = 0.5f;
			vPos.y += 1.0f * gSecondPerFrame * 9.8 * FallingSpeed * JumpAccel;
		}
	}
}

void Boss::SetAnimation()
{
	//Load Boss Animation Use File Loader
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

void Boss::PlayAnimation()
{
	if (AnimIndex >= Play.size()) 
	{
		AnimIndex = 0;// Loop back to the first frame
	}
	SetRect(Play[AnimIndex]);
	static float Time = 0.0f;
	float LifeTime = Play.size() * 0.1f;
	float frame = 0.0f;
	Time += gSecondPerFrame;
	frame = LifeTime / Play.size();
	if (Time >= frame)
	{
		AnimIndex++;

		CheckAnimationState();
		Time = Time - frame;
	}
}

void Boss::CheckAnimationState()
{
	//Set Animation Index for Loop
	if (AnimIndex >= Play.size()) 
	{
		if (Loop)
		{
			AnimIndex = 1;
		}
		else if (RandomState == GIGA)
		{
			AnimIndex = Play.size();
		}
		else
		{
			StateChange = true;

		}
		if (EndLoop)
		{
			AnimIndex = Play.size() - 1;
		}
	}
}

std::vector<Rect> Boss::FindSprite(std::wstring name)
{
	auto iter = FileList.find(name);
	if (iter != FileList.end())
	{
		return iter->second;
	}
	return std::vector<Rect>();
}

void Boss::SetSprite(std::vector<Rect> sprite)
{
	Play = sprite;
}

void Boss::SetPlayerPos(Vector2 pos)
{
	PlayerPos = pos;
}

void Boss::SetBossInverse()
{
	if ((WorldPos.x - PlayerPos.x) > 0)
	{
		Inverse = true;
	}
	else
	{
		Inverse = false;
	}
}

void Boss::SetLeftWallState(bool state)
{
	LeftWall = state;
}

void Boss::SetRightWallState(bool state)
{
	RightWall = state;
}

void Boss::SetGroundState(bool state)
{
	Ground = state;
}

void Boss::CreateBossBullet()
{
	// Create Boss Attack
	if (IsThrow == false)
	{
		Bullet = new BossBullet;
		Bullet->SetData(D3D11Device, D3D11Context, ClientRect);
		Bullet->Init();
		Bullet->SetBossInverse(Inverse);
		Bullet->SetSprite(Bullet->FindSprite(L"BossBullet.txt"));
		Bullet->SetStartIndex();

		SetBulletPos();
		GetBoomerang = false;
		IsThrow = true;
	}

}

void Boss::SetBulletPos()
{
	//Init Boss Bullet Pos
	if (Inverse)
	{
		Bullet->WorldPos.x = ObjectRect.Min.x - 15;
	}
	else
	{
		Bullet->WorldPos.x = ObjectRect.Max.x + 15;
	}
	Bullet->WorldPos.y = ObjectRect.Min.y + (ObjectRect.Size.y / 3.0f) - 10;
	Bullet->SetPosition(WorldPos);
}

void Boss::SetCameraData(Vector2 Campos, Vector2 CamSize)
{
	CameraPosition = Campos;
	CameraSize = CamSize;
}

bool Boss::IsShootBullet()
{
	return IsThrow;
}

Rect Boss::GetBulletRect()
{
	return Bullet->ObjectRect;
}

Rect Boss::GetObjectRect()
{
	return ObjectRect;
}

void Boss::GetDamage(float damage)
{
	CurrBossHp -= damage;
}

void Boss::SetBulletLeftHit(bool state)
{
	Bullet->SetLeftWallState(state);
}

void Boss::SetBulletRightHit(bool state)
{
	Bullet->SetRightWallState(state);
}

void Boss::RetrieveBullet()
{
	// If Boss Retrieve Bullet
	if (Bullet->IsTurn())
	{
		GetBoomerang = true;
		Bullet->Release(); // Release Bullet
		delete Bullet;
		Bullet = nullptr;
		IsThrow = false;
	}
}

void Boss::SpawnLaser()
{
	LaserSound->PlayEffect(0.75f); // Play Laser Effect
	for (int i = 0; i <= LaserCount; i++) // Loop LaserCount
	{
		Laser* BossLaser = new Laser;
		BossLaser->SetData(D3D11Device, D3D11Context, ClientRect);
		BossLaser->Init();
		BossLaser->SetSprite(BossLaser->FindSprite(L"BossGiga.txt"));
		LaserList.emplace_back(BossLaser);
		SetLaserPosition(i);
	}
}

void Boss::SetLaserPosition(int idx)
{
	if (GigaPattern == 0)
	{
		if (idx == 0)
		{
			LaserList[0]->SetPosition({ 100, (float)ClientRect.bottom / 2.0f - 100 });
		}
		else
		{
			LaserList[idx]->SetPosition({ LaserList[idx - 1]->WorldPos.x + 100, (float)ClientRect.bottom / 2.0f - 100 });
		}
	}
	else if (GigaPattern == 1)
	{
		if (idx == 0)
		{
			LaserList[0]->SetPosition({ 870, (float)ClientRect.bottom / 2.0f - 100 });
		}
		else
		{
			LaserList[idx]->SetPosition({ LaserList[idx - 1]->WorldPos.x - 100, (float)ClientRect.bottom / 2.0f - 100 });
		}
	}
	else
	{
		if (idx == 0)
		{
			LaserList[0]->SetPosition({ 100, (float)ClientRect.bottom / 2.0f - 100 });
		}
		else {
			LaserList[idx]->SetPosition({ LaserList[idx - 1]->WorldPos.x + 100, (float)ClientRect.bottom / 2.0f - 100 });
		}
	}
}

void Boss::LaserFrame()
{
	for (int i = LaserList.size() - 1; i >= 0; i--)
	{
		if (LaserList[i] != nullptr)
		{
			LaserList[i]->Frame();
			LaserList[i]->CameraSet(CameraPosition, CameraSize);
		}
	}
	LaserEndEvent();
}

void Boss::LaserRender()
{
	for (int i = LaserList.size() - 1; i >= 0; i--)
	{
		if (LaserList[i] != nullptr)
		{
			LaserList[i]->Render();
		}
	}
}

void Boss::LaserRelease()
{
	for (int i = 0; i < LaserList.size(); i++)
	{
		if (LaserList[i] != nullptr)
		{
			LaserList[i]->Release();
			delete LaserList[i];
			LaserList[i] = nullptr;
		}
	}
}

void Boss::LaserEndEvent()
{
	//Laser Attack End Event
	int count = 0;
	for (int i = 0; i < LaserList.size(); i++)
	{
		if (LaserList[i] != nullptr)
		{
			if (LaserList[i]->GetPlayEndState() == true)
			{
				LaserList[i]->Release();
				delete LaserList[i];
				LaserList[i] = nullptr;
				count++;
			}
		}
	}
	if (count == LaserList.size())
	{
		LaserList.clear();
	}
}

void Boss::CheckLaserCollision(Object2D* object)
{
	// Laser Attack Collision
	for (int i = LaserList.size() - 1; i >= 0; i--)
	{
		if (LaserList[i] != nullptr)
		{
			Rect laser_rect = LaserList[i]->GetObjectRect();
			Rect obj_rect = object->GetObjectRect(); // Get object2D Collision Rect
			if (Collision::RectToRect(laser_rect, obj_rect)) // Check Laser , Object Collision
			{
				object->GetDamage(LaserList[i]->GetLaserDamage());
			}
		}
	}
}