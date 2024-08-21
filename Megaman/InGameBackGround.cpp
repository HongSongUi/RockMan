#include "InGameBackGround.h"

bool InGameBackGround::Init()
{
	Load(L"../_shader/DefaultShader.txt", L"../_Texture/Background1.png");
	SetRect({ 0,0, 1000,700 });
	SetPosition({ 500,350 });
	CreateVertex();
    return true;
}
