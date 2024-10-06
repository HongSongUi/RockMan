#include "IntroText.h"

bool IntroText::Init()
{
    return true;
}

void IntroText::Move(int dir)
{
	Vector2 pos = WorldPos;
	pos.x += dir * gSecondPerFrame * Speed;
	SetPosition(pos);
}

void IntroText::SetTexture(std::wstring TextureName, std::wstring MaskName)
{
	Speed = 800.0f;
	SetMask(MaskName);
	Load(L"../_shader/DefaultMaskShader.txt", TextureName);
	SetRect({ 0,0,100,100 });
	CreateVertex();
}

void IntroText::SetTextureAndRect(std::wstring TextureName, std::wstring MaskName, Rect rt)
{
	SetTexture(TextureName, MaskName);
	SetRect(rt);
	Init();
}
