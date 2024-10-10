#include "Bullet.h"
#include "../TextLoader.h"
#include <fstream>

bool Bullet::Init()
{
    //Init Character Bullet Data
    AnimIndex = 0;
    Speed = 1000.f;
    NormalDamage = 5;
    SecondDamage = 10;
    FullDamage = 15;
    LifeTime = 0.75f;
    Loader = new TextLoader;
    Loader->LoadDir(L"../_Animation/Bullet/");
    AnimationInit();
    SetMask(L"../_Texture/Bustermask.bmp");
    Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/Buster.bmp");
    SetRect(GetAnimation(L"Normal.txt")[0]);
    CreateVertex();
    return true;
}

bool Bullet::Frame()
{
    LifeTime -= gSecondPerFrame;
    if (LifeTime < 0.f)
    {
        //Release();
    }
    Position = WorldPos;
    if (AnimIndex - 1 >= (CurrentAnimation.size() / 2.0f)) //Handle Move use Animation Index
    {
        MoveBullet();
    }
    Play();
    SetPosition(Position);
    
    return true;
}



bool Bullet::Release()
{
    Object2D::Release();
    delete Loader;
    return true;
}

Rect Bullet::GetObjectRect()
{
    return ObjectRect;
}

std::vector<Rect> Bullet::GetAnimation(std::wstring name)
{
    auto iter = AnimationFiles.find(name);
    if (iter != AnimationFiles.end()) 
    {
        return iter->second;
    }
    return std::vector<Rect>();
}

void Bullet::MoveBullet()
{
    if (Inverse) 
    {
        Position.x += 1.0f * gSecondPerFrame * Speed;
    }
    else 
    {
        Position.x += -1.0f * gSecondPerFrame * Speed;
    }
}

void Bullet::AnimationInit()
{
    //Load Bullet Animation 
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
        AnimationFiles.insert(std::make_pair(name, BulletList));
        BulletList.clear();
        file.close();
    }
}

void Bullet::Play()
{
    //Play Bullet Animation
    if (AnimIndex >= CurrentAnimation.size())
    {
        AnimIndex = 0;
    }
    SetRect(CurrentAnimation[AnimIndex]);

    float LifeTime = 0.2f;
    float frame = 0.0f;
    Time += gSecondPerFrame;
    frame = LifeTime / CurrentAnimation.size();
    if (Time >= frame)
    {
        AnimIndex++;
        if (AnimIndex >= CurrentAnimation.size()) 
        {
            if (CurrentAnimation.size() % 2 == 1) 
            {
                AnimIndex = (CurrentAnimation.size() / 2.0f) + 2;
            }
            else 
            {
                AnimIndex = (CurrentAnimation.size() / 2.0f) + 1;
            }
        }
        Time = Time - frame;
    }
}

void Bullet::SetAnimaiton(std::vector<Rect> texture)
{
    CurrentAnimation = texture;
}

void Bullet::SetType(BulletType type)
{
    Type = type;
    if (Type == NORMAL)
    {
        Damage = NormalDamage;
        SetAnimaiton(GetAnimation(L"Normal.txt"));
    }
    else if (Type == SEC_CHARGE)
    {
        Damage = SecondDamage;
        SetAnimaiton(GetAnimation(L"SecondCharge.txt"));
    }
    else if (Type == FULL_CHARGE)
    {
        Damage = FullDamage;
        SetAnimaiton(GetAnimation(L"FullCharge.txt"));
    }
}

void Bullet::SetSpawnPosition(Vector2 pos)
{
    SetPosition(pos);
}

void Bullet::SetInverse(bool inverse)
{
    Inverse = inverse;
}

bool Bullet::CheckBulletState()
{
    if (AnimIndex < (CurrentAnimation.size() / 2.0f) - 1)//Timing when the actual bullet is created in the Sprite
    {
        return true;
    }
    return false;
}

bool Bullet::CheckBulletTime()
{
    if (LifeTime < 0.f)
    {
        return true;
    }
    return false;
}

bool Bullet::ReadyToFire()
{
    if (AnimIndex < (CurrentAnimation.size() / 2.0f) - 1) //Timing when the actual bullet is created in the Sprite
    {
        return true;
    }
    return false;
}

BulletType Bullet::GetType()
{
    return Type;
}

float Bullet::GetBulletDamage()
{
    return Damage;
}

