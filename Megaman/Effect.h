#pragma once
#include <map>
#include "Object2D.h"

class TextLoader;


enum EffectTag 
{
	CHRGING,
	BOOSTER,
	WALLDUST,
	DASHDUST,
	BULLETHIT,
	EXPLOSION,
};


class Effect : public Object2D
{
	SpriteRect Range;
	std::vector<Rect> EffectList;
	TextLoader* Text;
	std::map<std::wstring, std::vector<Rect>> FileList;

	float EffectTimer = 0.0f;
	std::wstring TextureName;
	std::wstring MaskTexName;
	float WaitTime = randstep(0, 2);
public:
	int Index = 0;
	std::vector<Rect> Sprite;
	int Tag = 0;
	bool LoopStop = 0;
	bool Loop = false;
	bool EndLoop = false;
public:
	bool Init();
	bool Frame();
	bool Release();
public:
	std::vector<Rect> FindSprite(std::wstring name);
	void TextInit();
	void Play();
	void SetSprite(std::vector<Rect>& Texture);
	void SetTexture(std::wstring TextureName, std::wstring MaskName);
	void ResetEffect();
	void SetInverse(bool inverse);
	bool CheckAnimationEnd();
	void SetLoop(bool isLoop);
	void SetEndLoop(bool isEndLoop);
	void ResetAnimation();
};

