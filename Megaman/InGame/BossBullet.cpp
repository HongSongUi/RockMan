#include "BossBullet.h"
#include "../TextLoader.h"
#include "Boss.h"
#include <fstream>
bool BossBullet::Init()
{
	Speed = 500.0f;
	Damage = 15.f;
	Loader = new TextLoader;
	Loader->LoadDir(L"../_Animation/BossBullet/");
	TextInit();
	SetMask(L"../_Texture/DynamoBulletmask.bmp");
	Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/DynamoBullet.bmp");
	SetRect(FindSprite(L"BossBullet.txt")[0]);
	CreateVertex();
    return true;
}

bool BossBullet::Frame()
{
	Vector2 Pos = WorldPos;

	Pos.x += Dir * gSecondPerFrame * Speed;
	if (LeftWall == true) 
	{
		Dir = 1;
	}
	else if (RightWall == true) 
	{
		Dir = -1;
	}

	Play();
	SetPosition(Pos);
    return true;
}

bool BossBullet::Release()
{
	Object2D::Release();
	delete Loader;
	return true;
}

Rect BossBullet::GetObjectRect()
{
	return ObjectRect;
}

std::vector<Rect> BossBullet::FindSprite(std::wstring name)
{
	auto iter = FileList.find(name);
	if (iter != FileList.end())
	{
		return iter->second;
	}
	return std::vector<Rect>();
}

void BossBullet::TextInit()
{
	for (int i = 0; i < Loader->fileList.size(); i++) 
	{
		int num = 0;
		std::ifstream file(Loader->fileList[i]);
		std::wstring name = Loader->GetSplitName(Loader->fileList[i]);
		file >> num;
		BulletList.reserve(num);
		while (file.eof() == false)
		{
			file >> BulletRect.Left >> BulletRect.Top >> BulletRect.Right >> BulletRect.Bottom;
			BulletList.push_back({ BulletRect.Left,BulletRect.Top, BulletRect.Right,BulletRect.Bottom });
		}
		FileList.insert(std::make_pair(name, BulletList));
		BulletList.clear();
		file.close();
	}
}

void BossBullet::Play()
{
	if (Index >= Sprite.size()) {
		Index = 0;
	}
	SetRect(Sprite[Index]);
	float LifeTime = 0.2f;
	float frame = 0.0f;
	Time += gSecondPerFrame;
	frame = LifeTime / Sprite.size();
	if (Time >= frame)
	{
		Index++;

		Time = Time - frame;
	}
}

void BossBullet::SetSprite(std::vector<Rect> Texture)
{
	Sprite = Texture;
}

void BossBullet::SetBossInverse(bool BossInverse)
{
	BossInverse = BossInverse;
	if (BossInverse == true) 
	{
		Dir = -1;
	}
	else if (BossInverse == false) 
	{
		Dir = 1;
	}
}

void BossBullet::SetStartIndex()
{
	Index = 0;
}

void BossBullet::SetLeftWallState(bool state)
{
	LeftWall = state;
	CheckTurn = state;
}

void BossBullet::SetRightWallState(bool state)
{
	RightWall = state;
	CheckTurn = state;
}

bool BossBullet::IsTurn()
{
	return CheckTurn;
}

float BossBullet::GetBulletDamage()
{
	return Damage;
}


