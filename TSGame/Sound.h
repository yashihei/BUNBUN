#pragma once
#include <XAudio2.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

class Sound;
struct SoundBuffer;

class SoundManager {
public:
	SoundManager();
	~SoundManager();
	void load(std::string filePath, std::string alias);
	void play(std::string alias, float volume = 1.0f, float freqRatio = 1.0f, bool loop = false);
	void stop(std::string alias);
private:
	IXAudio2* m_xAudio;
	IXAudio2MasteringVoice* m_masteringVoice;
	std::unordered_map<std::string, std::shared_ptr<Sound>> m_sounds;
};

class Sound {
public:
	Sound(std::string filePath, IXAudio2* xAudio);
	~Sound();
	void init(bool loop = false);
	void play(float volume = 1.0f, float freqRatio = 1.0f);
	void stop();
private:
	IXAudio2* m_xAudio;
	IXAudio2SourceVoice* m_sourceVoice;
	std::shared_ptr<SoundBuffer> m_soundBuffer;
};

struct SoundBuffer {
	explicit SoundBuffer(std::string filePath);
	~SoundBuffer();
	DWORD size;
	WAVEFORMATEX waveFormatEx;
	std::vector<BYTE> buffer;
};
