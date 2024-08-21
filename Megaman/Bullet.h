#pragma once
#include <map>
#include "Object2D.h"

enum Owner
{
	PLAYER,
	NPC,
};

enum BulletType
{
	NORMAL,
	SEC_CHARGE,
	FULL_CHARGE,
};

class Bullet : public Object2D
{
	SpriteRect BulletRect;
	Object2D* Owner;
	std::vector<Rect> BulletList;
	class TextLoader* Loader;
	std::map<std::wstring, std::vector<Rect>> AnimationFiles;


	Vector2 Position;
	float Time;
	float Damage;
	float LifeTime;
	std::vector<Rect> CurrentAnimation;
	int AnimIndex;
	BulletType Type;
	float NormalDamage =10.f;
	float SecondDamage = 12.f;
	float FullDamage = 15.f;
public:
	bool Init()override;
	bool Frame() override;
	bool Release() override;
	virtual Rect GetObjectRect() override;
public:
	std::vector<Rect> GetAnimation(std::wstring name);
	void MoveBullet();
	void AnimationInit();
	void Play();
	void SetAnimaiton(std::vector<Rect>& texture);
	void SetType(BulletType type);
	void SetSpawnPosition(Vector2 pos);
	void SetInverse(bool inverse);
	bool CheckBulletState();
	bool CheckBulletTime();
	bool ReadyToFire();
	BulletType GetType();
	float GetBulletDamage();
};

