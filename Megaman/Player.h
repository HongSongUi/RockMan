#pragma once
#include "Object2D.h"
#include "State.h"
#include <list>
#include <map>

#define LEFT 0
#define RIGHT 1

enum ChargeState {
	NOCHARGE,
	CHARGING,
	CHARGEND,
};

class Player : public Object2D
{
	class TextLoader* Loader;
	class Bullet* PlayerBullet;
	class Effect* ChargeEffect;
	class Effect* DashDustEffect;
	class Effect* DashBoostEffect;
	class Effect* WallDustEffect;
	class Effect* BulletHitEffect;

	class GameSound* EffectSound;
	class GameSound* PlayerSound;
	class GameSound* ShootSound;
	class GameSound* ChargingSound;
	class GameSound* ChargeShootSound;
	class GameSound* DeathSound;
	class GameSound* BulletHitSound;
	class GameSound* VictorySound;
	class GameSound* ExitSound;

	State* CurrentState;
	bool IsGround = false;
	bool LeftWall = false;
	bool RightWall = false;
	bool IsJump;
	bool AnimLoop;
	bool AnimEndLoop;
	bool IsCharge;
	bool ChargeStart;
	bool PlayChargeSound;
	bool IntroMove;
	bool HitGround;
	ChargeState Charge;
	Vector2 Position;
	Vector2 BulletSpawnPoint;
	Vector2 CameraPosition;
	Vector2 CameraSize;

	SpriteRect Sprite;
	
	StateTag PrevTag;
	StateTag CurrentTag;

	std::map<std::wstring, std::vector<Rect>> FileList;
	std::vector<Rect> CurrentAnimation;
	std::vector<Rect> AnimationList;
	std::list<class Bullet*> BulletList;
	int	AnimationIndex;
	float JumpForce;
	float Speed = 350.f;
	float Time = 0.f;
	float MaxHealth = 100.f;
	float CurHealth = MaxHealth;
	float Y_Speed = 100.f;
	float VictoryTimer = 1.5f;
	float BulletCoolTime;
	float HitCoolTime;
	float CoolTime;
	float ChargeTime;
	float HealthScale;
	bool IsHit;
	bool CanHit;
	bool IntroFinish;
	bool MoveEnd;
	bool Intro;
	bool IsPauseAnimation;
	bool IsSpawn;
	bool MoveStart;
	bool IsGameStart;
	bool PlayerDeath;
	bool IsRender;
	bool PlayerWin;
	bool PlayerExit;
	bool GameEnd;
	bool PlayVictory;
	bool StopLoop;
	bool IsPlayBulletHitEffect;
	bool IsPlayDeathSound;
	bool IsPlayExitSound;
public:
	bool Init() override;
	bool Frame() override;
	bool IntroFrame();
	bool Render() override;
	bool Release() override; 
	virtual bool SetData(ID3D11Device* device, ID3D11DeviceContext* context, RECT clientRt) override;
	virtual void GetDamage(float damage)override;
	virtual Rect GetObjectRect() override;
	void Falling(float FallingSpeed = 100.f);
	void InitSound();
	bool GetIsGround();
	void SetChargeState(ChargeState state);
	void PlayAnimation();
	ChargeState GetChargeState();
	bool CheckAnimationIndex(int adjust = 0);
	void SetAnimation(std::vector<Rect>& Animation);
	void InitAnimation();
	std::vector<Rect> FindSprite(std::wstring name);
	void SetCurrentState();
	void ChangeState(State* NewState);
	void SetInverse(bool state);
	void SetJumpState(bool state);
	void SetAinmationIndex(int Idx);
	void JumpAction();
	void UpdateJumpSpeed(float Force = 0.5f);
	void HandleAnimationLoop();
	void ResetAnimIndex();
	void PlayerMove(int Dir);
	void PlayerDash(float PlayerSpeed);
	bool CheckJumpAccel();
	bool GetInverseState();
	void SetLeftWallState(bool state);
	void SetRightWallState(bool state);
	void SetGroundState(bool state);
	void SetFallingAnim();
	void SpawnBullet();
	void InitBullet();
	bool GetLeftWallState();
	bool GetRightWallState();
	void SetBulletSpawn();
	void SetBulletSpawnPoint(Bullet* bullet);
	void UpdateChargingState();
	void UpdateBullet(Vector2 CamPos, Vector2 CamSize);
	void BulletRender();
	void BulletRelease();
	void EffectInit();
	void PlayChargeEffect();
	void PlayDashEffect();
	void PlayWallEffect();
	void PlayBulletHitEffect();
	bool GetIsCharge();
	void ResetChargeState();
	void DashEndEvent();
	void HitEndEvent();
	void WallClingEndEvent();
	float GetChargeTime();
	void SetCameraData(Vector2 Campos, Vector2 CamSize);
	Vector2 GetPosition();
	
	
	void SetHitStartEvent();
	bool IsPlayerHit();
	bool IsPlayerCanHit();
	StateTag GetPrevState();
	void CheckHitCoolTime();
	bool CheckBulletCollision(Object2D* object);
	void SpawnBulletHitEffect(Bullet* bullet, Object2D* object);
	void SetPlaySound(std::wstring file_name);
	void IntroFalling();
	void PlayEffectSound(std::wstring file_name);
	void SpawnPlayerEvent();
	void PauseAnimation(int idx);
	void ReStartAnimation();
	bool IsAnimationEnd();
	bool GetIntroMoveEnd();
	void IntroWalk();
	void SetSpawnState(bool state);
	bool GetSpawnState();
	void SetStartIntroMove(bool state);
	bool GetStartIntroMove();
	void SetGameStart(bool is_start);
	bool GetIsGameStart();
	float GetHealthScale();
	bool IsPlayerDeath();
	void SetRenderState(bool state);
	void SetPlayerWin(bool state);
	bool GetPlayerWin();
	void SetPlayerExit(bool state);
	bool GetPlayerExit();
	void SetAnimationEndLoop();
	void MoveUp();
	bool GetGameEnd();
	void SetVitoryEvent();
	bool GetStopLoop();
	void ResetEndLoop();
};

