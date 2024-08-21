#pragma once
#include "Object2D.h"
#include <map>
#include <vector>
#include <list>


enum BossState 
{
    SWORD,
    GIGA,
    BOOMERANG,
    REFLECT,
    LOWHPIDEL,
    BOSSHIT,
    READY,
    DIE,
    BOSSINTRO,
};

class Boss : public Object2D
{
    class TextLoader* Loader;
	class GameSound* EffectSound;
	class GameSound* Voice;
	class GameSound* GigaCharge;
	class GameSound* ExplosionSound;
	class GameSound* LaserSound;

    class BossBullet* Bullet;
    SpriteRect Sprite;
    
    std::map<std::wstring, std::vector<Rect>> FileList;
    std::vector<Rect> AnimationList;
    std::vector<Rect> Play;
    std::vector<Rect> CurrIdle;
    std::vector<class Laser*> LaserList;
    std::vector<class Effect*> ExplosionList;

    int AnimIndex = 0;
    Vector2 PlayerPos;
    Vector2 vPos;
    Vector2 Dir;
    Vector2 CameraPosition;
    Vector2 CameraSize;

    int Rand = 0;
    int LaserCount = 3;
    float FallingSpeed = 100.0f;
    float GuardTime = 2.0f;
    float JumpAccel = 0.5f;
    float WaitTime = 1.0f;
    float CurrnetExplosionTime = 0.f;
    float ExplosionTime = 1.5f;
    float ExplosionWait = 0.f;
    float MinExploWait = 0.1f;
    float Damage = 10.f;
    bool Wait = true;
    bool StateChange = false;
    bool Loop = false;
    bool HitGround = false;
    float BossHp = 100.0f;
    float CurrBossHp = BossHp;
    float LaserTime = 0.25f;
    float OutroWait = 1.0f;
    
    bool EndLoop = false;

    float HealthScale = 0.0f;
    bool LeftWall = false;
    bool RightWall = false;
    bool Ground = false;
    BossState RandomState = READY;
    bool GetBoomerang = false;
    bool LoopStop = false;
    bool GigaAttack = false;
    bool GigaBullet = false;
    int GigaPattern = 0;
    bool Intro = true;
    bool IntroFinish = false;
    bool IsDeath = false;
    bool Return = false;
    bool Ending = false;
    bool IsThrow = false;
    bool DeathEvent = false;
    int EffectCount = 0;
    bool ExplosionStart = true;;
    bool Routine = false;
    bool Explosion = false;
    bool TimeWait = true;
    bool NextExplosion = false;
    bool IsBossDeath = false;
    bool IsEndEffect = false;
    bool IsPlayExitSound = false;
public:
    bool Init() override;
    bool Frame() override;
    void OutroFrame();
    bool Render() override;
    bool Release() override;
    virtual Rect GetObjectRect() override;
    virtual void GetDamage(float damage) override;
    void Update();
    void UpdateState(Vector2 pos);
    void SetPosition(Vector2 pos) override;
    void Falling();
    void SetAnimation();
    void PlayAnimation();
    void CheckAnimationState();
    std::vector<Rect> FindSprite(std::wstring name);
    void SetSprite(std::vector<Rect>& sprite);
    void SetPlayerPos(Vector2 pos);
    void SetBossInverse();
    void SetLeftWallState(bool state);
    void SetRightWallState(bool state);
    void SetGroundState(bool state);
    void CreateBossBullet();
    void SetBulletPos();
    void SetCameraData(Vector2 Campos, Vector2 CamSize);
    bool IsShootBullet();
    Rect GetBulletRect();
    void SetBulletLeftHit(bool state);
    void SetBulletRightHit(bool state);
    void RetrieveBullet();
    void SpawnLaser();
    void SetLaserPosition(int idx);
    void LaserFrame();
    void LaserRender();
    void LaserRelease();
    void LaserEndEvent();
    void CheckLaserCollision(Object2D* object);
    void SoundInit();
    void SetBossState(Vector2 pos);
    void IntroFrame();
    bool GetIntroEnd();
    float GetHealthScale();
    void EffectInit();
    void StartDeathEvent();
    void PlayExplosionEffect();
    void EffectRender();
    void StartExplosion();
    void UpdateExplosionAnimation();
    void UpdateExplosionFrame(class Effect* explosion);
    bool GetBossDeath();
    void ExplosionRelease();
    bool GetIsEffectEnd();
    bool GetIsEnding();
    Vector2 GetWorldPosition();
    bool GetInverse();
    BossState GetBossState();
    void StateReady();
    float GetBossDamage();
    float GetBulletDamage();
};

