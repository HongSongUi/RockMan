#include "IntroBackGround.h"

bool IntroBackGround::Init()
{
    Load(L"../_shader/DefaultShader.txt", L"../_Texture/BossIntroBackGround.png");
    SetPosition({ 500,350 });
    CreateVertex();
    return true;
}
