#pragma once
#include "Scene.h"
#include "TextureManager.h"
class TitleScene : public Scene
{
	class GameSound* BGM;
	class GameSound* Nar;
	class Title* GameTitle;
	class TitleUI*  GameTitleUI;
public:
	bool Init() override;
	bool Render()override;
	bool Release()override;
	bool Frame()override;
	void SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)override;
};

