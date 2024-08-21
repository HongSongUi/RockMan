#pragma once
#include <Windows.h>
class Scene
{
protected:
	class ID3D11Device* D3D11Device;
	class ID3D11DeviceContext* D3D11Context;
	RECT ClientRect;
public:
	bool ChangeScene = false;
public:
	virtual bool Init() = 0;
	virtual bool Render() = 0;
	virtual bool Release() = 0;
	virtual bool Frame() = 0;
	virtual void SetData(ID3D11Device* Device, ID3D11DeviceContext* Context, RECT ClientRt) = 0;
};

