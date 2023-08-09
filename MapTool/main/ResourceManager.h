#pragma once
#include "defalut.h"

class Texture;

class ResourceManager
{
public:
	static ResourceManager* m_pInstance;

public:
	ResourceManager();
	~ResourceManager();

public:
	void Init(ID3D11Device* _device);

public:
	void SaveTexture(ID3D11Device* _device, const wstring);
	Texture* FindTexture(const wstring _key);

private:
	Texture* m_Texture;
	map<wstring, Texture*> m_mapTextures;

	//wchar_t		m_szContentPath[255];
};

