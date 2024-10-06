#pragma once
#include <vector>
#include <string>
#include "../Scene.h"

class GameSound;
class StageSelect;
class StageUI;

class StageScene : public Scene
{
	GameSound* BGM;
	GameSound* ChooseSound;
	GameSound* SelectSound;
	
	StageSelect* Select;
	StageUI* StageSelectUI;
	
	std::vector<StageUI*> MugList;
	std::vector<StageUI*>_SelectMug;
	std::wstring name;
	float x = 150;
	float LeftY = 150;
	float RightY = 150;
	int Index = 0;
	bool Secret = false;
public:
	bool Init() override;
	bool Render()override;
	bool Release()override;
	bool Frame()override;
	void SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)override;
	void SetMugShot(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt);
};

