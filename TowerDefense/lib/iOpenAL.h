#pragma once

#pragma comment(lib, "OpenAL32.lib")

#include <al.h>
#include <alc.h>

class iOpenAL
{
public:
	iOpenAL(int audioNum);
	virtual ~iOpenAL();

public:
	void initBuffer(int idx, const char* szFormat, ...);
	void initSource(int idx, bool repeat);

	void play(int idx);
	void pause(int idx);
	void stop(int idx);
	void volume(int idx, float vol); // 0 ~ 1 : 0 (100 ) ~ 1 ( 100) OpenAL에서는 값을 0~1로 씀.


public:
	ALuint* gBuffer;
	ALuint* gSource;
	int bufCount;


};

struct AudioInfo
{
	char path[512];
	bool repeat;
	float volume;

};

void loadAudio(AudioInfo* ai , int aiNum); // 게임을 로딩할떄
void freeAudio(); // 종료할때
void pauseAudio(); // 게임을 일시정지할떄
void resumeAudio(); // 재개할때
void audioPlay(int sndIdx);
void audioStop();
void audioStop(int sndIdx);
void audioVolume(float bgm, float sfx, int sfxNum);


