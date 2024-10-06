#include "StageSelect.h"

bool StageSelect::Init()
{
	Load(L"../_shader/DefaultShader.txt", L"../_Texture/StageSelect.png");
	SetPosition({ 500,350 });
	CreateVertex();
	return true;
}
