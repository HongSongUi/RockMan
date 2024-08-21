#pragma once
#include "Scene.h"
#include <list>
class InGame : public Scene
{
	class Player* Character;
	class Boss* Dynamo;

	class InGameBackGround* BackGround;
	class Wall* Block;

	class Camera2D* PlayerCam;
	class Camera2D* BackGroundCam;

	class GameSound* BGM;
	class GameSound* Narration;
	class GameSound* ExplosionSound;
	class GameSound* WaringSound;


	class IntroText* ReadyText_R;
	class IntroText* ReadyText_L;
	class IntroText* Warning;
	class IntroText* Complete;
	class IntroText* Failed;

	class FadeInOut* Red;
	class FadeInOut* White;
	class FadeInOut* Black;

	class HealthBar* CharacterHealth;
	class HealthBar* CharacterUI;
	class HealthBar* DynamoHealth;
	class HealthBar* DynamoUI;

	const float EXPLOSION_TIME_THRESHOLD = 7.f;
	const float MAX_ALPHA = 1.f;
	const float FADE_SPEED = 0.25f;
	const float MAX_GLITTER_TIME = 9.f;

	bool ReadyTextFinish;
	bool Intro = true;
	bool Outro = false;
	bool FadeIn = false;
	bool FadeOut = false;
	bool GlitterStart = false;
	bool PlayReadyNar;
	bool Flash;
	bool RedFlashRender;
	float ReadyWait;
	float FlashTimer;
	float WaitTimer = 0.f;
	float GlitterAlpha = 0.f;
	float GlitterTime = 0.f;
public:
	bool Init();
	bool Frame();
	void IntroFrame();
	void OutroFrame();
	void MainFrame();
	void PlayerWinFrame();
	void PlayerLoseFrame();
	bool Render();
	bool Release();
	void TextInit();
	void TextRender();
	void TextRelease();
	void FadeObjectInit();
	void FadeObjectRender();
	void FadeObjectRelease();
	void HealthBarInit();
	void HealthBarFrame();
	void HealthBarRender();
	void HealthBarRelease();
	void SoundInit();
public:
	virtual void SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)override;
	void UpdateCamera();
	void CheckMapCollision();
	void CameraPositionAdjust();
	void CheckBossBulletCollision();
	void CheckBossToPlayerCollision();
	
	void HandleReadyText();
	void HandleReadyClean();

	bool HandleWaringText();
	void UpdateRedFade();
	void HandleWaringClean();
	void HandleGlitterAlpha();

};

