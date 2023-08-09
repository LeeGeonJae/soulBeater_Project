#include "ResourceManager.h"
#include "Texture.h"

ResourceManager* ResourceManager::m_pInstance = nullptr;

ResourceManager::ResourceManager()
	: m_mapTextures()
	, m_Texture(nullptr)
{
	m_pInstance = this;
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init(ID3D11Device* _device)
{
	SaveTexture(_device, L"Golem.png");
	SaveTexture(_device, L"Charactor.png");
}

void ResourceManager::SaveTexture(ID3D11Device* _device, const wstring _key)
{
	m_Texture = new Texture;
	m_Texture->Initialize(_device, _key);

	m_mapTextures.insert(make_pair(_key, m_Texture));
}

Texture* ResourceManager::FindTexture(const wstring _key)
{
	map<wstring, Texture*>::iterator iter = m_mapTextures.find(_key);

	if (iter != m_mapTextures.end())
		return iter->second;

	return nullptr;
}