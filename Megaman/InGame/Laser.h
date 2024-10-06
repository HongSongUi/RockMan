#pragma once
#include "Object2D.h"
#include <vector>
#include <map>
class Laser :public Object2D
{
	class TextLoader* Loader;
	SpriteRect Sprite;
	std::vector<Rect> LaserList;
	std::map<std::wstring, std::vector<Rect>> FileList;
	float Time = 0.0f;
	std::vector<Rect> Animaition;
	int Index = 0;
	bool Hit = false;
	bool PlayEnd = false;
	float Damage;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	std::vector<Rect> FindSprite(std::wstring name);
	void TextInit();
	void Play();
	void SetSprite(std::vector<Rect> texture);
	void SetPosition(Vector2 pos);
	bool GetPlayEndState();
	float GetLaserDamage();
};

