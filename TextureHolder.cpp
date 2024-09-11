#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	map<string, Texture>& m = m_s_Instance->m_Textures;
	map<string, Texture>::iterator keyValuePair = m.find(filename);

	if (keyValuePair != m.end())
	{	// Texture found
		return keyValuePair->second;
	}
	else
	{	// Texture not found
		Texture& texture = m[filename]; // auto&
		texture.loadFromFile(filename);
		return texture;
	}

}
