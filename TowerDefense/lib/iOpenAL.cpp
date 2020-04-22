#include "iOpenAL.h"

#include <al.h>
#include <alc.h>

class iOpenAL
{
public:
	iOpenAL();
	virtual ~iOpenAL();

public:
	void initBuffer(int idx, const char* szFormat, ...);
	void initSource(int idx, bool repeat);

	void play(int idx, bool repeat);
	void pause(int idx);
	void stop(int idx);
	void volume(int idx, float vol); // 0 ~ 1 : 0 (100 ) ~ 1 ( 100) OpenAL에서는 값을 0~1로 씀.


public:
	ALuint* gBuffer;
	ALuint* gSource;
	int bufCount;


};

static iOpenAL* al;
void loadAudio()
{
	al = new iOpenAL();
}

void freeAudio()
{
	delete al;
}

void pauseAudio()
{
	for (int i = 0; i < al->bufCount; i++)
		al->pause(i);
}

void resumeAudio()
{

}

void audioPlay(int sndIdx)
{
	//al->play(sndIdx, );
}

void audioStop()
{
	for(int i = 0 ; i<al->bufCount; i++)
		al->stop(i);
}

void audioVolume(float bgm, float sfx, int sfxNum)
{
	for (int i = 0; i < al->bufCount; i++)
		if (i < sfxNum)
			al->volume(i, sfx);
		else //if(i < bgm)
			al->volume(i, bgm);
}
