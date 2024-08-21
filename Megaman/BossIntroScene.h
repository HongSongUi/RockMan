#pragma once
#include <vector>
#include "Scene.h"
#include "ReSUtility.h"

class BossIntroScene : public Scene
{
	class GameSound* BGM;
	class GameSound* Nar;
	class FadeInOut* FadeObject;
	class IntroBackGround* BackGround;
	std::vector<class BossImage*> BossImageList;

	bool SceneChange = false;
	float SceneWaitTimer = 2.0f;
	float WaitTime = 0.2f;
	bool Wait = false;
	bool Flash = false;
	float CharShotTime = 2.5f;
	bool FlashEnd = false;
	//float ImageTimer = 1.0f;
	float FlashTimer = 2.5f;
	//int NextImage = 0;
	int ImgCount = 4;
public:
	virtual bool Init() override;
	virtual bool Render()override;
	virtual bool Release()override;
	virtual bool Frame()override;
	virtual void SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt)override;
	void PlayBGM();
	void MoveBossImage();
	void FlashScreen();
	void UpdateBossImage();
	void FlashScreenEnd();
	void HandleSceneChange();
};

