#pragma once
#include "BaseObject.h"
class FadeInOut : public BaseObject
{
public:
	virtual bool Init() override;
	virtual void FadeSet(float Alpha);
	virtual void SetTexture(std::wstring TextureName);
};

