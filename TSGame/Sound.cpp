#include "Sound.h"

#include <stdexcept>
#include <vector>

SoundManager::SoundManager() :
m_xAudio(NULL), m_masteringVoice(NULL)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	HRESULT hr = XAudio2Create(&m_xAudio, flags);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize XAudio2");

	hr = m_xAudio->CreateMasteringVoice(&m_masteringVoice);
	if (FAILED(hr))
		throw std::runtime_error("Error creating mastering voice");
}

SoundManager::~SoundManager()
{
	m_sounds.clear();
	if (m_masteringVoice)
		m_masteringVoice->DestroyVoice();
	if (m_xAudio)
		m_xAudio->Release();
	CoUninitialize();
}

void SoundManager::load(std::string filePath, std::string alias) {
	auto sound = std::make_shared<Sound>(filePath, m_xAudio);
	m_sounds[alias] = sound;
}

void SoundManager::play(std::string alias, float volume, bool loop) {
	m_sounds[alias]->init(loop);
	m_sounds[alias]->play(volume);
}

void SoundManager::stop(std::string alias) { 
	m_sounds[alias]->stop();
}

Sound::Sound(std::string filePath, IXAudio2* xAudio) : m_xAudio(xAudio), m_sourceVoice(NULL) {
	m_soundBuffer = std::make_shared<SoundBuffer>(filePath);
}

Sound::~Sound() {
	if (m_sourceVoice) {
		m_sourceVoice->Stop();
		m_sourceVoice->FlushSourceBuffers();
		m_sourceVoice->DestroyVoice();
	}
}

void Sound::init(bool loop) {
	//TODO:–³‘Ê‚È‚±‚Æ‚µ‚Ä‚È‚¢‚©’²¸
	//FIXME:DebugMode‚¾‚Æmastervoice->destroy‚ÅƒGƒ‰[“f‚­
	HRESULT hr = m_xAudio->CreateSourceVoice(&m_sourceVoice, &m_soundBuffer->waveFormatEx);
	if (FAILED(hr))
		throw std::runtime_error("Error creating source voice");

	XAUDIO2_BUFFER buffer = {};
	buffer.pAudioData = m_soundBuffer->buffer.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = m_soundBuffer->size;
	buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

	m_sourceVoice->SubmitSourceBuffer(&buffer);
}

void Sound::play(float volume) {
	m_sourceVoice->SetVolume(volume);
	m_sourceVoice->Start();
}

void Sound::stop() {
	m_sourceVoice->Stop();
}

SoundBuffer::SoundBuffer(std::string filePath) {
	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo = {};

	hMmio = mmioOpen(const_cast<LPSTR>(filePath.c_str()), &mmioInfo, MMIO_READ);
	if (!hMmio)
		throw std::runtime_error("Failed open " + filePath);
	
	MMRESULT mmResult;

	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmResult = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	//TODO:error check

	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmResult = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	//TODO:error check

	mmioRead(hMmio, (HPSTR)&waveFormatEx, formatChunk.cksize);
	//TODO:error check

	mmioAscend(hMmio, &formatChunk, 0);

	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	//TODO:error check

	buffer.resize(dataChunk.cksize);
	size = mmioRead(hMmio, (HPSTR)buffer.data(), dataChunk.cksize);
	//TODO:error check

	mmioClose(hMmio, 0);
}

SoundBuffer::~SoundBuffer() {
	buffer.clear();
}
