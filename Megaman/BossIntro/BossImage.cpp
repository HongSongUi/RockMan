#include "BossImage.h"

bool BossImage::Init()
{
	//init move speed
	Speed = 100.0f; 
    return true;
}

bool BossImage::Frame()
{
	return true;
}

void BossImage::SetImage(std::wstring TextureName)
{
	//Set Boss Image use texture name
	Load(L"../_shader/DefaultShader.txt", TextureName);
	SetRect({ 0,0,200,417 }); // Set Image Range
	SetPosition({ 1400,350 }); // Set World Position
	CreateVertex(); 
}

void BossImage::ImageMove()
{
	Vector2 pos = WorldPos;
	pos.x += -1.0f * gSecondPerFrame * 500.0f;
	SetPosition(pos);
}

void BossImage::SetMasking(std::wstring MaskName, std::wstring TextureName)
{
	SetMask(MaskName);
	Load(L"../_shader/DefaultMaskShader.txt", TextureName);
	SetPosition({ 1400,350 });
	CreateVertex();
}
