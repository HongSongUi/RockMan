#include "Wall.h"

bool Wall::Init()
{
	SetMask(L"../_Texture/Background02mask.bmp");
	Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/BackGround02.bmp");
	SetRect({ 0,0, 508,359 });
	SetPosition({ 500,300 });
	LeftWall.SetRect(0, 0, 80, 700);
	RightWall.SetRect(920, 0, 1000, 700);
	Ground.SetRect(0, 500, 1000, 700);
	CreateVertex();
    return true;
}
