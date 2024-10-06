#include "HealthBar.h"
#include "../TextLoader.h"
#include <fstream>

bool HealthBar::Init()
{
	Text = new TextLoader;

	Text->LoadDir(L"../_Animation/Hud/");
	TextInit();
	SetMask(L"../_Texture/HeathBarmask.bmp");
	Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/HeathBar.bmp");
	SetRect(FindSprite(L"X_Health.txt")[0]);
	CreateVertex();
    return true;
}

bool HealthBar::Frame()
{
    return true;
}

void HealthBar::UpdateRect(float Hp)
{
}

void HealthBar::TextInit()
{
	for (int i = 0; i < Text->fileList.size(); i++)
	{
		int num = 0;
		std::ifstream file(Text->fileList[i]);
		std::wstring name = Text->GetSplitName(Text->fileList[i]);
		file >> num;
		HpList.reserve(num);
		while (file.eof() == false) 
		{
			file >> HpBar.Left >> HpBar.Top >> HpBar.Right >> HpBar.Bottom;
			HpList.push_back({ HpBar.Left,HpBar.Top, HpBar.Right,HpBar.Bottom });
		}
		FileList.insert(std::make_pair(name, HpList));
		HpList.clear();
		file.close();
	}
}

std::vector<Rect> HealthBar::FindSprite(std::wstring name)
{
	auto iter = FileList.find(name);
	if (iter != FileList.end()) 
	{
		return iter->second;
	}
	return std::vector<Rect>();
}
