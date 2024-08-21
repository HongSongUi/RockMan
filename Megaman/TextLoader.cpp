#include "TextLoader.h"

#include <Windows.h>
#include <tchar.h>
#include <corecrt_io.h>
void TextLoader::LoadDir(std::wstring path)
{
	std::wstring dirpath = path + L"*.*";
	intptr_t handle;
	struct _wfinddata_t data;
	handle = _wfindfirst(dirpath.c_str(), &data);
	if (handle == -1L) return;
	do 
	{
		if ((data.attrib & _A_SUBDIR) && data.name[0] != '.') 
		{
			LoadDir(path + data.name + L"/");
		}
		else if (data.name[0] != '.')
		{
			fileList.push_back(path + data.name);
		}
	} 
	while (_wfindnext(handle, &data) == 0);

	_findclose(handle);
}

std::wstring TextLoader::GetSplitName(std::wstring path)
{
	std::wstring name;
	TCHAR drive[MAX_PATH] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	TCHAR filename[MAX_PATH] = { 0, };
	TCHAR ext[MAX_PATH] = { 0, };
	_tsplitpath_s(path.c_str(), drive, dir, filename, ext);
	name = filename;
	name += ext;
	return name;
}
