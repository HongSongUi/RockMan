#include "TitleUI.h"
#include "SoundManager.h"
#include "Input.h"
#include "TextLoader.h"
#include <fstream>
bool TitleUI::Init()
{
	SoundMgr.LoadAll(L"../_sound/SFX/");
	Loader = new TextLoader();
	Loader->LoadDir(L"../_Animation/UI/");
	TextInit();
	SetMask(L"../_Texture/TitleUImask.bmp");
	Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/TitleUI.bmp");

	SetRect(FindSprite(L"TitleUi01.txt")[0]);
	SelectSound = SoundMgr.Find(L"ROCK_X5_00169.wav");
	ChooseSound = SoundMgr.Find(L"ROCK_X5_00546.wav");
	CreateVertex();

	return true;
}

void TitleUI::SetTexture(std::vector<Rect> TextureName)
{
	Sprite = TextureName;
}

bool TitleUI::Frame()
{
	if (UiChange == false) 
	{
		if (GameInput.GetKey(VK_RETURN) == KEY_PUSH) 
		{
			SetRect(FindSprite(L"TitleUi02.txt")[0]);
			SelectSound->Play();
			UiChange = true;
		}
	}
	else if (UiChange == true) 
	{
		if (GameInput.GetKey(VK_RETURN) == KEY_PUSH)
		{
			SelectSound->Play();
			if (Index == 0) {
				ChangeScene = true;
			}
		}
		if (GameInput.GetKey(VK_DOWN) == KEY_PUSH)
		{
			ChooseSound->Play();
			Index++;
			if (Index >= 2)
			{
				Index = 2;
			}
			SetRect(FindSprite(L"TitleUi02.txt")[Index]);
		}
		else if (GameInput.GetKey(VK_UP) == KEY_PUSH)
		{
			ChooseSound->Play();
			Index--;
			if (Index <= 0)
			{
				Index = 0;
			}
			SetRect(FindSprite(L"TitleUi02.txt")[Index]);
		}
	}
	return true;
}

std::vector<Rect> TitleUI::FindSprite(std::wstring name)
{
	auto iter = FileList.find(name);
	if (iter != FileList.end()) 
	{
		return iter->second;
	}
	return std::vector<Rect>();
}

void TitleUI::TextInit()
{
	for (int i = 0; i < Loader->fileList.size(); i++) 
	{
		int num = 0;
		std::ifstream file_stream(Loader->fileList[i]);
		std::wstring name = Loader->GetSplitName(Loader->fileList[i]);
		file_stream >> num;
		UIList.reserve(num);
		while (file_stream.eof() == false) {
			file_stream >> InGameUI.left >> InGameUI.top >> InGameUI.right >> InGameUI.bottom;
			UIList.push_back({ InGameUI.left,InGameUI.top, InGameUI.right,InGameUI.bottom });
		}
		FileList.insert(std::make_pair(name, UIList));
		UIList.clear();
		file_stream.close();
	}
}
