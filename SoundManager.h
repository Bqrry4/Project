#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>

enum class Soundtype {
	Music,
	Chunk
};

class Sound {

public:
	virtual ~Sound() = default;
	virtual bool Load(const char* path) = 0;
	virtual void Play() = 0;
};

class Music : public Sound
{
	Mix_Music* Sound;
public:
	Music() : Sound(nullptr)
	{}
	~Music()
	{
		Mix_FreeMusic(Sound);
	}
	bool Load(const char* path)
	{
		Sound = Mix_LoadMUS(path);
		return Sound;
	}
	void Play()
	{
		Mix_PlayMusic(Sound, -1);
	}
	void Pause() {
		Mix_PauseMusic();
	}
};

class Chunk : public Sound
{
	Mix_Chunk* Sound;
public:
	Chunk() : Sound(nullptr)
	{}
	~Chunk() {
		Mix_FreeChunk(Sound);
	}
	bool Load(const char* path)
	{
		Sound = Mix_LoadWAV(path);
		return Sound;
	}
	void Play()
	{
		Mix_PlayChannel(-1, Sound, 0);
	}
};


class SoundManager {

	std::vector<Sound*> SoundMap;

	SoundManager()
	{}
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

public:
	~SoundManager()
	{
		Clean();
	}

	inline static SoundManager& GetInstance() { static SoundManager instance; return instance; }
	bool Load(const char* path, Soundtype type); 

	Sound* getSound(__int8 id);
	void Clean();
};