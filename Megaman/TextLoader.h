#pragma once
#include <vector>
#include "ReSUtility.h"

class TextLoader
{
public:
	std::vector<std::wstring> fileList;
public:
	void LoadDir(std::wstring path);
	std::wstring GetSplitName(std::wstring path);
};

