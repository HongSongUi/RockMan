#pragma once
#include <map>
#include "Object2D.h"

class HealthBar : public Object2D
{
	class TextLoader* Text;
	SpriteRect HpBar;
	std::vector<Rect> HpList;
	std::map<std::wstring, std::vector<Rect>> FileList;

public:
	bool Init();
	bool Frame();
	void UpdateRect(float Hp);
	void TextInit();
	std::vector<Rect> FindSprite(std::wstring name);
};

