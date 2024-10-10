#include "Sample.h"
#include "SoundManager.h"
#include "../TitleScene/TitleScene.h"
#include "StageScene.h"
#include "BossIntroScene.h"
#include "InGame.h"
bool Sample::Init()
{
    Is2DGame = true;
    SoundMgr.LoadAll(L"../_sound/");
  
    TitleStage = new TitleScene;
    SelectStage = new StageScene;
    IntroScene = new BossIntroScene;
    InGameScene = new InGame;

    //Manage Scenes use List
    TitleStage->SetData(GameCore::GetDevice(), GameCore::GetContext(), GameCore::GetClientRect());
    SceneList.emplace_back(TitleStage);
    SelectStage->SetData(GameCore::GetDevice(), GameCore::GetContext(), GameCore::GetClientRect());
    SceneList.emplace_back(SelectStage);
    IntroScene->SetData(GameCore::GetDevice(), GameCore::GetContext(), GameCore::GetClientRect());
    SceneList.emplace_back(IntroScene);
    InGameScene->SetData(GameCore::GetDevice(), GameCore::GetContext(), GameCore::GetClientRect());
    SceneList.emplace_back(InGameScene);
    SceneIndex = 0;
    return true;
}

bool Sample::Frame()
{
    SceneList[SceneIndex]->Frame();
    if (SceneList[SceneIndex]->ChangeScene == true) 
    {
        SceneIndex++;
        if (SceneIndex >= SceneList.size()) 
        {
            SceneIndex = SceneIndex - 1;
        }
    }
    return true;
}

bool Sample::Render()
{
    SceneList[SceneIndex]->Render();
    return true;
}

bool Sample::Release()
{
    for (int i = 0; i < SceneList.size(); i++) {
        SceneList[i]->Release();
        delete SceneList[i];
        SceneList[i] = nullptr;
    }
    return true;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 전역 문자열을 초기화합니다.
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
   // LoadStringW(hInstance, IDC_WINAPI01, szWindowClass, MAX_LOADSTRING);

    Sample  MyWin;
    MyWin.SetWindow(hInstance, L"Megaman", 1000, 700);
    MyWin.Run();

    return 1;
}
