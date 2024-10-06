#pragma once
#include "Object2D.h"
#include <map>
class BossBullet : public Object2D
{
	class TextLoader* Loader;
	SpriteRect BulletRect;
	std::vector<Rect> BulletList;
	std::map<std::wstring, std::vector<Rect>> FileList;
	float Time = 0.0f;
	int Dir = -1.0f;
	std::vector<Rect> Sprite;
	int Index = 0;
	bool LeftWall = false;
	bool RightWall = false;
	bool CheckTurn = false;
	bool BossInverse = false;
	bool HitPlayer = false;
	float Damage;
public:
	bool Init() override;
	bool Frame() override;
	bool Release() override;
	virtual Rect GetObjectRect() override;
	std::vector<Rect> FindSprite(std::wstring name);
	void TextInit();
	void Play();
	void SetSprite(std::vector<Rect> Texture);
	void SetBossInverse(bool BossInverse);
	void SetStartIndex();
	void SetLeftWallState(bool state);
	void SetRightWallState(bool state);
	bool IsTurn();
	float GetBulletDamage();
};

