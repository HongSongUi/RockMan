#include "TitleScene.h"
#include "Title.h"
#include "TitleUI.h"
#include "SoundManager.h"

bool TitleScene::Init()
{
	Nar = SoundMgr.Find(L"ROCK_X5_00587.wav");
	BGM = SoundMgr.Find(L"X5OP_0000.mp3");
	Nar->Play();
	GameTitle->Init();
	GameTitleUI->Init();
	return true;
}

bool TitleScene::Render()
{
	GameTitle->Render();
	
	GameTitleUI->MaskRender();
	return true;
}

bool TitleScene::Release()
{
	GameTitle->Release();
	GameTitleUI->Release();
	return true;
}

bool TitleScene::Frame()
{
	BGM->Play();
	
	GameTitleUI->Frame();

	GameTitleUI->SetPosition({ 500,500 });
	GameTitleUI->ShowObject();
	//GameTitleUI->ScreenToNdc();
	//GameTitleUI->UpdateVertexList();
	if (GameTitleUI->ChangeScene == true) 
	{
		BGM->Stop();
		ChangeScene = true;
		GameTitleUI->ChangeScene = false;
	}
	else 
	{
		ChangeScene = false;
	}
	return true;
}

void TitleScene::SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)
{
	D3D11Device = Device;
	D3D11Context = Context;
	GameTitle = new Title;
	GameTitleUI = new TitleUI;
	GameTitle->SetData(Device, Context, ClientRt);
	GameTitleUI->SetData(Device, Context, ClientRt);
	Init();
}
