#pragma once
#include "Object2D.h"
class IntroText : public Object2D
{
public:
	bool Init();
	void Move(int dir);
	void SetTexture(std::wstring TextureName, std::wstring MaskName);
	void SetTextureAndRect(std::wstring TextureName, std::wstring MaskName, Rect rt);
};

