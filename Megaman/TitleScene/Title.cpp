#include "Title.h"

bool Title::Init()
{
	Load(L"../_shader/DefaultShader.txt", L"../_Texture/Title.png");
	SetPosition({ 500,350 });
	CreateVertex();
	return true;
}
