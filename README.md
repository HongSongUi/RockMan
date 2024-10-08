# RockManX5(모작)

<img src ="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white"> <img src ="https://img.shields.io/badge/Direct_X-006600?style=for-the-badge&logo=directx&logoColor=black"> <img src ="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white"> <img src="https://img.shields.io/badge/fmod-000000?style=for-the-badge&logo=fmod&logoColor=white">

[日本の方はこちらへ](./README.JP.md)

## Source

[코드 보기](https://github.com/HongSongUi/RockMan/tree/master/Megaman)

## Game Overview
이 게임은 **메가맨X5**의 다이나모와의 전투를 재현한 작품으로, 원작의 다이나믹한 전투 스타일과 공격 패턴을 충실히 재현했습니다. 플레이어는 긴장감 넘치는 보스 전투를 경험할 수 있습니다.

본 게임은 [3D Engine](https://github.com/HongSongUi/Engine)으로 제작되었으며, 록맨의 움직임과 보스 전투 메커니즘을 구현하는 데 중점을 두었습니다. 다양한 전투 패턴과 전략적 플레이를 통해 도전과 성취감을 제공합니다.

본 프로젝트는 순수 **학습 목적**으로 제작된 것으로, 상업적 이용은 일절 없음을 명확히 밝힙니다.

## Features
- **다이나모 보스 전투 재현**: 메가맨X5의 다이나모 보스와 동일한 전투 패턴과 전략을 구현.
- **정밀한 캐릭터 조작**: 원작과 유사한 록맨의 움직임과 점프, 대시 등의 액션을 구현.　[FSM](https://github.com/HongSongUi/RockMan/tree/master/Megaman/FSM)
- **다양한 공격 패턴**: 다이나모의 다양한 공격 기술을 재현하여, 원작처럼 도전적인 보스 전투를 경험할 수 있음.
- **Sprite Animation**: 스프라이트를 활용한 부드러운 애니메이션으로 몰입감 제공.


## Setting
게임을 플레이하기 위해선 세팅이 필요합니다.

1. 실행 파일의 상위 폴더에 _Animation, _lib, _shader, _sound, _Texture 폴더가 있어야 합니다.

<img src ="https://github.com/user-attachments/assets/0cd8db05-fb8a-433e-af5e-655bde7dd8d9">

2. 실행 파일과 같은 폴더에 fmod.dll 이 있어야 합니다. (fmod.dll 은 _lib폴더에 있습니다.)

<img src ="https://github.com/user-attachments/assets/451ed600-6ba0-47be-9a40-9289b3c9b04a">

## How To Play
- **이동**: 방향키 좌, 우
- **앉기**: 방향키 아래
- **공격**: 키보드 X
- **모으기**: 키보드 X를 홀드
- **대쉬**: 키보드 Z
- **점프**: 키보드 C
- **메뉴 결정**: 키보드 Enter
- **메뉴 이동**: 방향키


## Screenshot
|  |   |
|---|---|
|메인 화면|보스 선택|
|<img width="450" alt="image" src="https://github.com/user-attachments/assets/5a00f104-b546-4c04-8af7-8c59e939db61">|<img width="450" alt="image" src="https://github.com/user-attachments/assets/aba6effc-d574-4dd9-8f3b-01750b60b610">|
|보스 인트로|전투|
|<img width="450" alt="image" src="https://github.com/user-attachments/assets/8c84129d-ea3a-4a37-9fd7-98affc551817">|<img width="450" alt="image" src="https://github.com/user-attachments/assets/98172b1e-4c9f-4cbd-b138-48c4943297b6">|
