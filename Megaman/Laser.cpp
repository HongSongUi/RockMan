#include "Laser.h"
#include "TextLoader.h"
#include <fstream>
bool Laser::Init()
{
   
    Loader = new TextLoader;
    Loader->LoadDir(L"../_Animation/BossBullet/");
    SetMask(L"../_Texture/Gigamask.bmp");
    Load(L"../_shader/DefaultMaskShader.txt", L"../_Texture/Giga.bmp");
    TextInit();
    SetRect(FindSprite(L"BossGiga.txt")[0]);
    CreateVertex();
    Damage = 20.f;
    return true;
}

bool Laser::Frame()
{
    if (PlayEnd == false) {
        Play();
    }
    if (Index >= Animaition.size() / 2.0f) 
    {
        Hit = false;
    }
    else 
    {
        Hit = true;
    }
    return true;
}

bool Laser::Render()
{
    if (PlayEnd == false)
    {
        MaskRender();
    }

    return true;
}

std::vector<Rect> Laser::FindSprite(std::wstring name)
{
    auto iter = FileList.find(name);
    if (iter != FileList.end())
    {
        return iter->second;
    }
    return std::vector<Rect>();
}

void Laser::TextInit()
{
    for (int i = 0; i < Loader->fileList.size(); i++)
    {
        int num = 0;
        std::ifstream file(Loader->fileList[i]);
        std::wstring name = Loader->GetSplitName(Loader->fileList[i]);
        file >> num;
        LaserList.reserve(num);
        while (file.eof() == false)
        {
            file >> Sprite.Left >> Sprite.Top >> Sprite.Right >> Sprite.Bottom;
            LaserList.push_back({ Sprite.Left,Sprite.Top, Sprite.Right,Sprite.Bottom });
        }
        FileList.insert(std::make_pair(name, LaserList));
        LaserList.clear();
        file.close();
    }
}

void Laser::Play()
{
    if (Index >= Animaition.size() - 1)
    {
        PlayEnd = true;
    }
    if (PlayEnd == false) {
        SetRect(Animaition[Index]);
        float LifeTime = 0.6f;
        float frame = 0.0f;
        Time += gSecondPerFrame;
        frame = LifeTime / Animaition.size();
        if (Time >= frame)
        {
            Index++;

            Time = Time - frame;
        }
    }
}

void Laser::SetSprite(std::vector<Rect>& texture)
{
    Animaition = texture;
}

void Laser::SetPosition(Vector2 pos)
{
    WorldPos = pos;


    Size.y = TextureRt.Size.y + 150;
    Size.x = TextureRt.Size.x + 20;


    ObjectRect.SetRect(WorldPos, Size);
}

bool Laser::GetPlayEndState()
{
    return PlayEnd;
}

float Laser::GetLaserDamage()
{
    return Damage;
}
