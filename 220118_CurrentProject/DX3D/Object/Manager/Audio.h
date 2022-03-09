#pragma once

class Audio : public Singleton<Audio>
{
private:
	friend class Singleton;

	struct SoundInfo
	{
		Sound* sound;
		Channel* channel;

		SoundInfo() : sound(nullptr), channel(nullptr)
		{
		}

		~SoundInfo()
		{
			sound->release();
		}
	};

	System* soundSystem;

	map<string, SoundInfo*> sounds;

	const int MAX_CHANNEL = 15;

	Audio();
	~Audio();

public:
	void Update();

	void Add(string key, string file, bool bgm = false);

	void Play(string key, float value = 1.0f);
	void Stop(string key);
	void Pause(string key);
	void Resume(string key);

	bool IsPlaySound(string key);
};