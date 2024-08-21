#include "FadeInOut.h"

bool FadeInOut::Init()
{
    Load(L"../_shader/DefaultAlphaShader.txt", L"../_Texture/GlitterEffect.png");
    CreateVertex();
    return true;
}

void FadeInOut::FadeSet(float Alpha)
{
	VertexList[0].Color = { 1.0f,1.0f,1.0f, Alpha };
	VertexList[1].Color = { 1.0f,1.0f,1.0f,Alpha };
	VertexList[2].Color = { 1.0f,1.0f,1.0f,Alpha };
	VertexList[3].Color = { 1.0f,1.0f,1.0f,Alpha };
	Mesh.SetVertexList(VertexList);
	Mesh.UpdateVertexBuffer();
}

void FadeInOut::SetTexture(std::wstring TextureName)
{
	Load(L"../_shader/DefaultAlphaShader.txt", TextureName);
	CreateVertex();
}
