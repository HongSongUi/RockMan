#include "BossIntroScene.h"
#include "BossImage.h"
#include "IntroBackGround.h"
#include "../FadeInOut.h"
#include "SoundManager.h"
#include "TextureManager.h"
bool BossIntroScene::Init()
{
	BGM = SoundMgr.Find(L"STAGE_START.mp3");
	Nar = SoundMgr.Find(L"ROCK_X5_00855.wav");
	FadeObject->Init();
	FadeObject->SetPosition({ 500, 350 });
	BossImageList[0]->SetImage(L"../_Texture/Dynamo1.png");
	BossImageList[0]->SetRect({ 0,0,200,417 });

	BossImageList[1]->SetImage(L"../_Texture/Dynamo2.png");
	BossImageList[1]->SetRect({ 0,0,200,700 });

	BossImageList[2]->SetMasking(L"../_Texture/Dynamo02mask.bmp", L"../_Texture/Dynamo02.bmp");
	BossImageList[2]->SetRect({ 0,0 ,57,81 });

	BossImageList[3]->SetMasking(L"../_Texture/DynamoNamemask.bmp", L"../_Texture/DynamoName.bmp");
	BossImageList[3]->SetRect({ 0,0 ,146,34 });

	BackGround->Init();
	return true;
}

bool BossIntroScene::Render()
{
	
	BackGround->Render();
	BossImageList[0]->Render();
	BossImageList[1]->Render();

	BossImageList[2]->MaskRender();

	BossImageList[3]->MaskRender();

	if (Flash == true)
	{
		FadeObject->Render();
	}

	return true;
}

bool BossIntroScene::Release()
{
	FadeObject->Release();
	BackGround->Release();
	for (int idx = 0; idx < BossImageList.size(); idx++)
	{
		BossImageList[idx]->Release();
	}
	return true;
}

bool BossIntroScene::Frame()
{

	PlayBGM();
	MoveBossImage();
	FlashScreen();
	UpdateBossImage();
	FlashScreenEnd();
	HandleSceneChange();



	return true;
}

void BossIntroScene::SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)
{
	D3D11Context = Context;
	BackGround = new IntroBackGround;
	BackGround->SetData(Device, Context, ClientRt);

	BossImageList.resize(ImgCount);
	for (int idx = 0; idx < ImgCount; idx++)
	{
		BossImage* Img = new BossImage;
	
		BossImageList[idx] = Img;
		BossImageList[idx]->SetData(Device, Context, ClientRt);
	}
	FadeObject = new FadeInOut;
	FadeObject->SetData(Device, Context, ClientRt);
	BGM = new GameSound;
	Nar = new GameSound;
	Init();
}

void BossIntroScene::PlayBGM()
{
	BGM->Play();
}

void BossIntroScene::MoveBossImage()
{
	if (BossImageList[0]->WorldPos.x >= 850)
	{
		BossImageList[0]->ImageMove();
	}
	else if (BossImageList[0]->WorldPos.x <= 850)
	{
		Flash = true;
	}
}

void BossIntroScene::FlashScreen()
{
	if (!Flash)
	{
		return;
	}
	FlashTimer -= gSecondPerFrame;
	if (Wait == false)
	{
		FadeObject->FadeSet(0);
		WaitTime -= gSecondPerFrame;
		if (WaitTime < 0.0f)
		{
			Wait = true;
			WaitTime = 0.2f;
		}
	}
	else
	{
		FadeObject->FadeSet(1);
		WaitTime -= gSecondPerFrame;
		BossImageList[1]->SetPosition({ 200,350 });
		if (WaitTime < 0.0f)
		{
			Wait = false;
			WaitTime = 0.2f;
		}
	}
	if (FlashTimer < 0.0f)
	{
		Flash = false;
		FlashEnd = true;
	}
}

void BossIntroScene::UpdateBossImage()
{
	for (int idx = 0; idx < BossImageList.size(); idx++)
	{
		BossImageList[idx]->ShowObject();
	}
}

void BossIntroScene::FlashScreenEnd()
{
	if (!FlashEnd)
	{
		return;
	}

	CharShotTime -= gSecondPerFrame;
	if (CharShotTime < 0.0f)
	{
		if (SceneChange == false)
		{
			Nar->Play();
		}
		SceneChange = true;
		BossImageList[2]->SetPosition({ 500, 350 });
		BossImageList[3]->SetPosition({ 500 ,500 });
	}
}

void BossIntroScene::HandleSceneChange()
{
	if (!SceneChange)
	{
		return;
	}

	SceneWaitTimer -= gSecondPerFrame;
	if (SceneWaitTimer < 0.0f)
	{
		ChangeScene = true;
	}
	
}
