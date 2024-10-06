#include "StageUI.h"
#include "../TextLoader.h"
#include <fstream>

bool StageUI::Init()
{
	Loader = new TextLoader();

	Loader->LoadDir(L"../_Animation/BossMugShot/");
	TextInit();
	//SetMask(L"../_Texture/BossMugShot.bmp");
	Load(L"../_shader/DefaultShader.txt", L"../_Texture/BossMugShot.bmp");
	SetRect(FindSprite(L"Boss01.txt")[0]);
	CreateVertex();
    return true;
}

bool StageUI::Frame()
{
    return true;
}

std::vector<Rect> StageUI::FindSprite(std::wstring name)
{
	auto iter = FileList.find(name);
	if (iter != FileList.end())
	{
		return iter->second;
	}
	return std::vector<Rect>();
}

void StageUI::TextInit()
{
	for (int i = 0; i < Loader->fileList.size(); i++) {
		int num = 0;
		std::ifstream file(Loader->fileList[i]);
		std::wstring name = Loader->GetSplitName(Loader->fileList[i]);
		file >> num;
		BossImgList.reserve(num);
		while (file.eof() == false) {
			file >> Mug.Left >> Mug.Top >> Mug.Right >> Mug.Bottom;
			BossImgList.push_back({ Mug.Left,Mug.Top, Mug.Right,Mug.Bottom });
		}
		FileList.insert(std::make_pair(name, BossImgList));
		BossImgList.clear();
		file.close();
	}
}

void StageUI::ChangeUi(bool Change, std::wstring name)
{
	if (Change == true) 
	{
		SetRect(FindSprite(name)[1]);
	}
	else 
	{
		SetRect(FindSprite(name)[0]);
	}
}
