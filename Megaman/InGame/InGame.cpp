#include "InGame.h"
#include "Player.h"
#include "Boss.h"
#include "InGameBackGround.h"
#include "Wall.h"
#include "Camera2D.h"
#include "Bullet.h"
#include "../FadeInOut.h"
#include "IntroText.h"
#include "HealthBar.h"
#include "SoundManager.h"
//#include "Collision.h"
bool InGame::Init()
{
	// Init InGame Data
	PlayReadyNar = true;
	Flash = false;
	RedFlashRender = false;
	ReadyWait = 1.f;
	FlashTimer = 2.f;
	WaitTimer = 0.5f;
	Character->Init();
	Dynamo->Init();

	BackGround->Init();
	Block->Init();
	PlayerCam->SetData({ Character->WorldPos.x + 250,Character->WorldPos.y - 200 }, { 800,600 }); // Set Player Camera
	//Set BackGround Camera Data
	BackGroundCam->SetData(PlayerCam->Position, { PlayerCam->CameraSize.x + (float)ClientRect.right ,  PlayerCam->CameraSize.y + (float)ClientRect.bottom });
	Character->SetCameraData(PlayerCam->Position, PlayerCam->CameraSize);
	Dynamo->SetCameraData(PlayerCam->Position, PlayerCam->CameraSize);
	TextInit();
	FadeObjectInit();
	HealthBarInit();
	SoundInit();
	return true;
}

bool InGame::Frame()
{
	if (Intro == true && Outro == false)
	{
		IntroFrame();
		return true;
	}
	else if (Intro == false && Outro == false)
	{
		MainFrame();
	}
	else
	{
		OutroFrame();
	}
	return true;
}

bool InGame::Render()
{
	BackGround->Render();
	Block->MaskRender();
	Dynamo->Render();
	Character->Render();
	HealthBarRender();
	TextRender();
	FadeObjectRender();
	return true;
}

bool InGame::Release()
{
	Character->Release();
	BackGround->Release();
	Block->Release();
	Dynamo->Release();
	HealthBarRelease();
	TextRelease();
	FadeObjectRelease();
	return true;
}

void InGame::TextInit()
{
	// Init  Text Image (Warning , Ready , Complete, Failed)
	ReadyText_L->SetTexture(L"../_Texture/Ready.bmp", L"../_Texture/Readymask.bmp");
	ReadyText_L->SetRect({ 0,0,200,50 });
	ReadyText_L->SetPosition({ -211,(float)ClientRect.bottom / 2.0f });

	ReadyText_R->SetTexture(L"../_Texture/Ready.bmp", L"../_Texture/Readymask.bmp");
	ReadyText_R->SetRect({ 0,0,200,50 });
	ReadyText_R->SetPosition({ 1211,(float)ClientRect.bottom / 2.0f });

	Warning->SetTexture(L"../_Texture/WarningText.bmp", L"../_Texture/WarningTextmask.bmp");
	Warning->SetRect({ 0,0,300,50 });
	Warning->SetPosition({ 450,350 });

	Complete->SetTexture(L"../_Texture/MissionComplete.bmp", L"../_Texture/MissionCompletemask.bmp");
	Complete->SetRect({ 0,0,512,128 });
	Complete->SetPosition({ 450,350 });

	Failed->SetTexture(L"../_Texture/MissionFailed.bmp", L"../_Texture/MissionFailedmask.bmp");
	Failed->SetRect({ 0,0,512,128 });
	Failed->SetPosition({ 450,350 });
}

void InGame::TextRender()
{
	// If Text != nullptr -> Render
	if (ReadyText_L != nullptr && ReadyText_R != nullptr)
	{
		ReadyText_L->MaskRender();
		ReadyText_R->MaskRender();
	}

	if (Warning != nullptr)
	{
		Warning->MaskRender();
	}
	if (!Dynamo->GetBossDeath() && Outro == true)
	{
		Failed->MaskRender();
	}
	if (Character->GetGameEnd())
	{
		Complete->MaskRender();
	}
}

void InGame::TextRelease()
{
	if (ReadyText_L)
	{
		ReadyText_L->Release();
	}
	if (ReadyText_R)
	{
		ReadyText_R->Release();
	}
	if (Warning)
	{
		Warning->Release();
	}
	if (Complete)
	{
		Complete->Release();
	}
	if (Failed)
	{
		Failed->Release();
	}
}

void InGame::FadeObjectInit()
{
	// Init FadeIn, FadeOut Image (Warning, Explosion)
	Red->SetTexture(L"../_Texture/FadeWarning.png");
	White->SetTexture(L"../_Texture/GlitterEffect.png");
}

void InGame::FadeObjectRender()
{
	if (RedFlashRender)
	{
		Red->Render();
	}
	if (GlitterStart)
	{
		White->Render();
	}
}

void InGame::FadeObjectRelease()
{
	Red->Release();
	White->Release();
}

void InGame::HealthBarInit()
{
	//Init Health Bar UI
	CharacterHealth->Init();
	DynamoHealth->Init();

	CharacterUI->Init();
	DynamoUI->Init();

	CharacterUI->SetRect(CharacterUI->FindSprite(L"X_Health.txt")[0]);
	DynamoUI->SetRect(DynamoUI->FindSprite(L"BossHealth.txt")[0]);

	CharacterHealth->SetRect(CharacterHealth->FindSprite(L"HealthBar.txt")[0]);
	DynamoHealth->SetRect(DynamoHealth->FindSprite(L"HealthBar.txt")[0]);

	CharacterUI->SetPosition({ 50,200 });// UI Set Position
	DynamoUI->SetPosition({ 950,200 });
}

void InGame::HealthBarFrame()
{
	//Update Player, Boss Health Bar
	float CharacterScale = Character->GetHealthScale();
	float BossScale = Dynamo->GetHealthScale();

	CharacterUI->ScreenToNdc();
	CharacterUI->UpdateVertexList();
	DynamoUI->ScreenToNdc();
	DynamoUI->UpdateVertexList();

	// Update player health bar rectangle based on health scale
	CharacterHealth->SetRect({ CharacterHealth->FindSprite(L"HealthBar.txt")[0].Min.x
		,CharacterHealth->FindSprite(L"HealthBar.txt")[0].Min.y + (CharacterScale * CharacterHealth->FindSprite(L"HealthBar.txt")[0].Size.y)
			,CharacterHealth->FindSprite(L"HealthBar.txt")[0].Max.x
		,CharacterHealth->FindSprite(L"HealthBar.txt")[0].Max.y });

	// Set player health bar position
	CharacterHealth->SetPosition({ 42,156 + (CharacterScale * CharacterHealth->FindSprite(L"HealthBar.txt")[0].Size.y) });
	CharacterHealth->ScreenToNdc();
	CharacterHealth->UpdateVertexList();

	// Update boss health bar rectangle based on health scale

	DynamoHealth->SetRect({ DynamoHealth->FindSprite(L"HealthBar.txt")[0].Min.x
		,DynamoHealth->FindSprite(L"HealthBar.txt")[0].Min.y + (BossScale * DynamoHealth->FindSprite(L"HealthBar.txt")[0].Size.y)
		,DynamoHealth->FindSprite(L"HealthBar.txt")[0].Max.x
		,DynamoHealth->FindSprite(L"HealthBar.txt")[0].Max.y });
	// Set boss health bar position
	DynamoHealth->SetPosition({ 939,151 + (BossScale * DynamoHealth->FindSprite(L"HealthBar.txt")[0].Size.y) });
	DynamoHealth->ScreenToNdc();
	DynamoHealth->UpdateVertexList();
}

void InGame::HealthBarRender()
{
	CharacterUI->MaskRender();
	DynamoUI->MaskRender();
	CharacterHealth->MaskRender();
	DynamoHealth->MaskRender();
}

void InGame::HealthBarRelease()
{
	CharacterUI->Release();
	DynamoUI->Release();
	CharacterHealth->Release();
	DynamoHealth->Release();
}

void InGame::SoundInit()
{
	BGM = new GameSound;
	Narration = new GameSound;
	ExplosionSound = new GameSound;
	WaringSound = new GameSound;

	BGM = SoundMgr.Find(L"DYNAMO.mp3");
	Narration = SoundMgr.Find(L"ROCK_X5_00554.wav");
	ExplosionSound = SoundMgr.Find(L"ROCK_X5_00157.wav");
	WaringSound = SoundMgr.Find(L"ROCK_X5_00287.wav");
}

void InGame::SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)
{
	ClientRect = ClientRt;
	Character = new Player;
	Dynamo = new Boss;
	BackGround = new InGameBackGround;
	Block = new Wall;
	PlayerCam = new Camera2D;
	BackGroundCam = new Camera2D;
	ReadyText_L = new IntroText;
	ReadyText_R = new IntroText;
	Warning = new IntroText;
	Complete = new IntroText;
	Failed = new IntroText;
	Red = new FadeInOut;
	White = new FadeInOut;
	CharacterHealth = new HealthBar;
	CharacterUI = new HealthBar;
	DynamoHealth = new HealthBar;
	DynamoUI = new HealthBar;


	Character->SetData(Device, Context, ClientRt);
	Dynamo->SetData(Device, Context, ClientRt);
	BackGround->SetData(Device, Context, ClientRt);
	Block->SetData(Device, Context, ClientRt);

	ReadyText_L->SetData(Device, Context, ClientRt);
	ReadyText_R->SetData(Device, Context, ClientRt);
	Warning->SetData(Device, Context, ClientRt);
	Complete->SetData(Device, Context, ClientRt);
	Failed->SetData(Device, Context, ClientRt);

	Red->SetData(Device, Context, ClientRt);
	White->SetData(Device, Context, ClientRt);

	CharacterHealth->SetData(Device, Context, ClientRt);
	DynamoHealth->SetData(Device, Context, ClientRt);

	CharacterUI->SetData(Device, Context, ClientRt);
	DynamoUI->SetData(Device, Context, ClientRt);
	Init();
}

void InGame::UpdateCamera()
{
	//Update Camera Use Character World Position
	PlayerCam->SetData({ Character->WorldPos.x + 250  ,Character->WorldPos.y - 200 }, PlayerCam->CameraSize);
	BackGroundCam->SetData(PlayerCam->Position, BackGroundCam->CameraSize);
}

void InGame::CheckMapCollision()
{

	Character->SetLeftWallState(Collision::RectToRect(Character->ObjectRect, Block->LeftWall));// Chracter Rect , Left Wall Rect
	Character->SetRightWallState(Collision::RectToRect(Character->ObjectRect, Block->RightWall)); // Character Rect , Right Wall Rect
	Character->SetGroundState(Collision::RectToRect(Character->ObjectRect, Block->Ground)); // Character Rect , Ground Rect

	Dynamo->SetLeftWallState(Collision::RectToRect(Dynamo->ObjectRect, Block->LeftWall)); // Boss Rect, Left Wall Rect
	Dynamo->SetRightWallState(Collision::RectToRect(Dynamo->ObjectRect, Block->RightWall)); // Boss Rect, Right Wall Rect
	Dynamo->SetGroundState(Collision::RectToRect(Dynamo->ObjectRect, Block->Ground)); // Boss Rect , Ground Rect
}

void InGame::CameraPositionAdjust()
{
	//Adjust Camera Position to avoid Leaving Game Screen
	if (PlayerCam->Max.x >= BackGround->ObjectRect.Max.x)
	{
		PlayerCam->Position.x = PlayerCam->Position.x - (PlayerCam->Max.x - BackGround->ObjectRect.Max.x);
	}
	else if (PlayerCam->Min.x <= BackGround->ObjectRect.Min.x)
	{
		PlayerCam->Position.x = PlayerCam->Position.x + (BackGround->ObjectRect.Min.x - PlayerCam->Min.x);
	}
	if (PlayerCam->Min.y <= BackGround->ObjectRect.Min.y)
	{
		PlayerCam->Position.y = PlayerCam->Position.y + (BackGround->ObjectRect.Min.y - PlayerCam->Min.y);
	}
	else if (PlayerCam->Max.y >= BackGround->ObjectRect.Max.y)
	{
		PlayerCam->Position.y = PlayerCam->Position.y - (PlayerCam->Max.y - BackGround->ObjectRect.Max.y);
	}
	PlayerCam->SetData(PlayerCam->Position, PlayerCam->CameraSize);
}

void InGame::CheckBossBulletCollision()
{
	//Boss Bullet Collision Check
	if (Dynamo->IsShootBullet())
	{
		Rect boss_bullet = Dynamo->GetBulletRect();
		Rect player_rect = Character->GetObjectRect();
		Rect boss_rect = Dynamo->GetObjectRect();
		if (Collision::RectToRect(boss_bullet, Block->LeftWall)) // bullet Rect , Left Wall Rect
		{
			Dynamo->SetBulletLeftHit(true);
		}
		else if (Collision::RectToRect(boss_bullet, Block->RightWall)) // bullet rect , right rect
		{
			Dynamo->SetBulletRightHit(true);
		}
		if (Collision::RectToRect(boss_bullet, player_rect)) // bullet rect , player rect
		{
			Character->GetDamage(Dynamo->GetBulletDamage());
		}
		if (Collision::RectToRect(boss_bullet, boss_rect)) // bullet rect , boss rect
		{
			Dynamo->RetrieveBullet();
		}
	}
}

void InGame::CheckBossToPlayerCollision()
{
	//check collision boss , character
	Rect player_rect = Character->GetObjectRect();
	Rect boss_rect = Dynamo->GetObjectRect();
	if (Collision::RectToRect(boss_rect, player_rect)) // boss rect , player rect
	{
		Character->GetDamage(Dynamo->GetBossDamage());
	}
}

void InGame::IntroFrame()
{
	// start intro frame
	if (Intro == false)
	{
		return;
	}
	Character->Frame(); // play character intro frame
	Character->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	Character->SetCameraData(PlayerCam->Position, PlayerCam->CameraSize);
	BackGround->CameraSet(PlayerCam->Position, BackGroundCam->CameraSize);
	Block->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	if (Character->GetSpawnState()) // if player spawn
	{
		HandleReadyText(); // render ready text
	}

	if (Character->GetIntroMoveEnd()) // if player move end
	{
		if (HandleWaringText()) // render waring text
		{
			Dynamo->IntroFrame(); // boss intro frame
			Dynamo->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
			Dynamo->SetCameraData(PlayerCam->Position, PlayerCam->CameraSize);
		}
	}
	if (Dynamo->GetIntroEnd()) // if intro end -> Game start
	{
		Intro = false;
		Character->SetGameStart(true);
	}
	CheckMapCollision();
	UpdateCamera();
	CameraPositionAdjust();
}

void InGame::OutroFrame()
{
	// play outro frame (player death or player win)
	BGM->Stop();
	if (Dynamo->GetBossDeath()) // check player win or death
	{
		PlayerWinFrame();
	}
	else
	{
		PlayerLoseFrame();
	}


	BackGround->CameraSet(PlayerCam->Position, BackGroundCam->CameraSize);
	Block->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);

	CheckMapCollision();
	UpdateCamera();
	CameraPositionAdjust();
}

void InGame::PlayerWinFrame()
{
	//if player win
	Character->SetPlayerWin(true);
	Dynamo->Frame(); //play boss death event
	Dynamo->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	if (Dynamo->GetIsEffectEnd())
	{
		HandleGlitterAlpha();
	}
	if (GlitterTime >= MAX_GLITTER_TIME)
	{
		Dynamo->OutroFrame();
		Dynamo->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	}
	if (Dynamo->GetIsEnding())
	{
		Character->SetPlayerExit(true);
	}
	Character->Frame(); // play player outro frame
	Character->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	if (Character->GetGameEnd())
	{
		Complete->ScreenToNdc();
		Complete->UpdateVertexList();
	}
}

void InGame::PlayerLoseFrame()
{
	//if player lose event
	Character->Frame();
	Character->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	Character->SetCameraData(PlayerCam->Position, PlayerCam->CameraSize);
	if (Character->IsPlayerDeath())
	{
		Failed->ScreenToNdc();
		Failed->UpdateVertexList();
	}
}

void InGame::MainFrame()
{
	// Main Frame of Game
	BGM->Play();
	BGM->SetVolume(0.25f);
	CheckMapCollision(); // Check Map object , (boss, player) collision
	BackGround->CameraSet(PlayerCam->Position, BackGroundCam->CameraSize);

	Character->Frame();
	Character->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	Character->SetCameraData(PlayerCam->Position, PlayerCam->CameraSize);

	Dynamo->UpdateState(Character->GetPosition());
	Dynamo->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);
	Dynamo->SetCameraData(PlayerCam->Position, PlayerCam->CameraSize);

	Block->CameraSet(PlayerCam->Position, PlayerCam->CameraSize);

	HealthBarFrame();

	UpdateCamera();

	CameraPositionAdjust();

	CheckBossBulletCollision(); 

	Character->CheckBulletCollision(Dynamo); // if character shoot bullet

	Dynamo->CheckLaserCollision(Character); // if boss pattern is Laser

	CheckBossToPlayerCollision();

	if (Character->IsPlayerDeath() || Dynamo->GetBossDeath())
	{
		Outro = true;
	}
}



void InGame::HandleReadyText()
{
	// Ready Text Move
	if (ReadyText_L == nullptr)
	{
		return;
	}
	if (ReadyText_L->WorldPos.x <= 500)
	{
		ReadyText_L->Move(1);
		ReadyText_L->ScreenToNdc();
		ReadyText_L->UpdateVertexList();
	}

	if (ReadyText_R->WorldPos.x >= 500)
	{
		ReadyText_R->Move(-1);
		ReadyText_R->ScreenToNdc();
		ReadyText_R->UpdateVertexList();
	}

	if (ReadyText_L->WorldPos.x >= 500 && ReadyText_R->WorldPos.x <= 500)
	{
		ReadyTextFinish = true;
		if (PlayReadyNar)
		{
			Narration->Play();
		}
		PlayReadyNar = false;
	}
	if (ReadyTextFinish)
	{
		HandleReadyClean();
	}
}

void InGame::HandleReadyClean()
{
	//if Ready Event End -> Release Ready Text
	ReadyWait -= gSecondPerFrame;
	if (ReadyWait < 0.0f)
	{
		if (ReadyText_L)
		{
			ReadyText_L->Release();
			delete ReadyText_L;
			ReadyText_L = nullptr;
		}
		if (ReadyText_R)
		{
			ReadyText_R->Release();
			delete ReadyText_R;
			ReadyText_R = nullptr;
		}
		Character->SetStartIntroMove(true);
	}
}

bool InGame::HandleWaringText()
{
	//Update Warning Text, event
	if (Warning == nullptr)
	{
		return true;
	}
	Warning->ScreenToNdc();
	Warning->UpdateVertexList();

	FlashTimer -= gSecondPerFrame;
	//sound
	UpdateRedFade();
	if (FlashTimer < 0.f)
	{
		HandleWaringClean();
		return true;
	}
	return false;
}

void InGame::UpdateRedFade()
{
	//Glitter event
	RedFlashRender = true;
	WaringSound->Play();
	if (Flash == false)
	{
		Red->FadeSet(0);
		WaitTimer -= gSecondPerFrame;
		if (WaitTimer < 0.f)
		{
			Flash = true;
			WaitTimer = 0.5f;
		}
	}
	else
	{
		Red->FadeSet(0.5f);
		WaitTimer -= gSecondPerFrame;
		if (WaitTimer < 0.f)
		{
			Flash = false;
			WaitTimer = 0.5f;
		}
	}
}

void InGame::HandleWaringClean()
{
	if (Warning != nullptr)
	{
		Warning->Release();
		delete Warning;
		Warning = nullptr;
		RedFlashRender = false;
	}
}

void InGame::HandleGlitterAlpha()
{
	//boss explosion glitter event
	GlitterTime += gSecondPerFrame;
	if (GlitterAlpha < MAX_ALPHA && !FadeOut)
	{
		FadeIn = true;
		FadeOut = false;
	}
	else if (GlitterAlpha >= MAX_ALPHA && FadeIn)
	{
		FadeOut = true;
		FadeIn = false;
		ExplosionSound->PlayEffect(0.5f);
	}

	if (FadeIn && !FadeOut)
	{
		GlitterAlpha += gSecondPerFrame * FADE_SPEED;
	}
	else if (!FadeIn && FadeOut) 
	{
		GlitterAlpha -= gSecondPerFrame * FADE_SPEED;
	}
	White->FadeSet(GlitterAlpha);
	GlitterStart = true;
}




