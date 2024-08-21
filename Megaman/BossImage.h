#pragma once
#include "Object2D.h"
class BossImage : public Object2D
{
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual void SetImage(std::wstring TextureName);
	virtual void ImageMove();
	virtual void SetMasking(std::wstring MaskName, std::wstring TextureName);
};

