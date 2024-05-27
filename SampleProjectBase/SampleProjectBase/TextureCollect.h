#pragma once
#include "ResourceCollect_Base.h"
#include "Singleton_Base.h"

class Texture;
class TextureCollect :
	public  ResourceCollect_Base<Texture>, public Singleton_Base<TextureCollect>
{
	friend class Singleton_Base<TextureCollect>;

public:
	// “Ç‚İ‚ñ‚Å”z—ñ‚É“ü‚ê‚é
	bool Load(const char* _pathName, std::string _texName);
};

