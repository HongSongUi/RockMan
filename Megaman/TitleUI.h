#pragma once
#include <map>
#include "Object2D.h"
struct UI
{
	float left;
	float top;
	float right;
	float bottom;
};
class TitleUI : public Object2D
{
	class GameSound* SelectSound;
	class GameSound* ChooseSound;
	class TextLoader* Loader;
	struct UI InGameUI;
	std::vector<Rect> UIList;
	std::map<std::wstring, std::vector<Rect>> FileList;

	int Index = 0;
	std::vector<Rect> Sprite;

public:
	bool ChangeScene = false;
	bool UiChange = false;
public:
	bool Init();
	void SetTexture(std::vector<Rect> TextureName);
	bool Frame();
	std::vector<Rect> FindSprite(std::wstring name);
	void TextInit();
};

