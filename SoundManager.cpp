#include "SoundManager.h"
#include "Game.h"


bool SoundManager::Load(const char* path, Soundtype type)
{
	if (path == nullptr)
	{
		Game::Log << "Invalid saound path";
		return false;
	}


	Sound* sound = (type == Soundtype::Music) ? ((Sound*)new Music) : ((Sound*)new Chunk);

	sound->Load(path);
	SoundMap.push_back(sound);
	return true;
}

Sound* SoundManager::getSound(__int8 id)
{
	if (id >= 0 && id < SoundMap.size())
	{
		return SoundMap[id];
	}
	return nullptr;
}

void SoundManager::Clean()
{
	for (Sound* sound : SoundMap)
	{
		delete sound;
	}
	SoundMap.clear();
}