#include "StageScene.h"
#include "StageSelect.h"
#include "StageUI.h"
#include "SoundManager.h"
#include "Input.h"
bool StageScene::Init()
{
	BGM = new GameSound;
	ChooseSound = new GameSound;
	SelectSound = new GameSound;

    BGM = SoundMgr.Find(L"BGM_004.mp3");
    ChooseSound = SoundMgr.Find(L"ROCK_X5_00546.wav");
    SelectSound = SoundMgr.Find(L"ROCK_X5_00661.wav");
    Select->Init();

    return true;
}

bool StageScene::Render()
{
	Select->Render();
	for (int i = 0; i < MugList.size(); i++) {
		MugList[i]->Render();
	}
    return true;
}

bool StageScene::Release()
{
	for (int i = 0; i < MugList.size(); i++) {
		if (MugList[i]) {
			MugList[i]->Release();
			delete MugList[i];
			MugList[i] = nullptr;
		}
	}
	Select->Release();
    return true;
}

bool StageScene::Frame()
{
	BGM->Play();
	for (int i = 0; i < MugList.size(); i++) 
	{
		name = L"Boss0";
		name += std::to_wstring(i + 1);
		name += L".txt";

		if (i == Index) 
		{
			MugList[i]->ChangeUi(true, name);
		}
		else 
		{

			MugList[i]->ChangeUi(false, name);
		}
		MugList[i]->ShowObject();
		
	}
	if (Secret == true) 
	{
		MugList[8]->SetRect(MugList[8]->FindSprite(L"Boss09.txt")[2]);
		MugList[8]->ShowObject();
		
		if (GameInput.GetKey(VK_RETURN) == KEY_PUSH)
		{
			SelectSound->Play();
			BGM->Stop();
			ChangeScene = true;
		}
	}
	else if (Secret == false)
	{
		if (GameInput.GetKey(VK_DOWN) == KEY_PUSH)
		{
			ChooseSound->Play();
			if (Index < MugList.size() - 1) 
			{
				Index++;
			}
		}
		else if (GameInput.GetKey(VK_UP) == KEY_PUSH) 
		{
			ChooseSound->Play();
			if (Index > 0) 
			{
				Index--;
			}
		}
		else if (GameInput.GetKey(VK_RIGHT) == KEY_PUSH) 
		{
			ChooseSound->Play();
			if (Index < MugList.size() / 2.0f)
			{
				Index += 4;
			}
		}
		else if (GameInput.GetKey(VK_LEFT) == KEY_PUSH) 
		{
			ChooseSound->Play();
			if (Index > MugList.size() / 2.0f - 1) 
			{
				Index -= 4;
			}
		}
		else if (GameInput.GetKey(VK_RETURN) == KEY_PUSH) 
		{
			ChooseSound->Play();
			if (Index == 8) 
			{
				Secret = true;
			}
		}
	}
    return true;
}

void StageScene::SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)
{
	D3D11Device = Device;
	D3D11Context = Context;
	ClientRect = ClientRt;
    Select = new StageSelect;
	Select->SetData(Device, Context, ClientRt);
	SetMugShot(Device, Context, ClientRt);
	Init();
}

void StageScene::SetMugShot(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)
{
	for (int i = 1; i < 10; i++) {
		StageSelectUI = new StageUI;
		StageSelectUI->SetData(Device, Context, ClientRt);
		StageSelectUI->Init();
		name = L"Boss0";
		name += std::to_wstring(i);
		name += L".txt";
		StageSelectUI->SetRect(StageSelectUI->FindSprite(name)[0]);
		if (i < 5) {
			StageSelectUI->SetPosition({ x,LeftY });
			LeftY += 150;
		}
		else {
			x = 850;
			StageSelectUI->SetPosition({ x,RightY });
			RightY += 150;
		}
		if (i == 9) {
			StageSelectUI->SetPosition({ (float)ClientRt.right / 2.0f,(float)ClientRt.bottom / 2.0f });
		}
		MugList.push_back(StageSelectUI);
	}
}
