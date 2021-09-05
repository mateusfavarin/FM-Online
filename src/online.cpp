#include <time.h>
#include <Windows.h>
#include "fm.h"
#include "memory.h"
#define HALF_FRAME 8 // game runs at 60fps, so half a frame is 8ms

clock_t start = clock();
clock_t end = clock();
int timeElapsed;

void WaitHalfFrame()
{
	end = clock();
	timeElapsed = end - start;
	if (timeElapsed < HALF_FRAME)
		Sleep(HALF_FRAME - timeElapsed);
	start = clock();
}

bool InitOnline(string gameName)
{
	if (!GetProcessSettings(gameName, "mednafen.exe"))
		return false;
	return true;
}

void InitDuel()
{
	HookMaxLPBypass();
	HookDuelistCodeBypass();
	while (!SceneIsPvP())
		WaitHalfFrame();
	SetSeed();
	RemoveTrapRotation();
}

void Play(char player)
{
	InitDuel();
	while (true)
	{
		WaitHalfFrame();
		HandleGame(player);
	}
}