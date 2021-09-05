#include <iostream>
#include <Windows.h>
#include "memory.h"

using namespace std;

const int numTrapRotInst = 3;
const int numDrawCursorInst = 3;
const int numPlayers = 2;

namespace RAM
{
	/* ASM instructions */
	uintptr_t maxLPDigits[numPlayers] = { 0x016F98, 0x016F14 };
	uintptr_t playerBeginMatch = 0x017AF2;
	uintptr_t trapRot[numTrapRotInst] = { 0x01CA24, 0x01CA2C, 0x01CA30 };
	uintptr_t maxLP = 0x0251A4;
	uintptr_t triangleText = 0x0284D8;
	uintptr_t triangleImage = 0x02946C;
	uintptr_t playGsSound = 0x0370E4;
	uintptr_t drawGsCursor[numDrawCursorInst] = { 0x0370EC, 0x0371D0, 0x03725C };
	uintptr_t drawGsSymbols = 0x037FF4;
	uintptr_t drawGsText = 0x039730;
	uintptr_t duelistCode = 0x03FAE8;
	uintptr_t duelistCodeRet = 0x03FAF0;

	/* Data */
	uintptr_t currentTurn = 0x09B1D5;
	uintptr_t currentScene = 0x09B26C;
	uintptr_t cardView[numPlayers] = { 0x0EA00F, 0x0EA02F };
	uintptr_t lp[numPlayers] = { 0x0EA004, 0x0EA024 };
	uintptr_t prng = 0x0FE6F8;
}

namespace Cheat
{
	/* ASM Instructions */
	char maxLPDigits = 0x05;
	char minLPDigits = 0x04;
	char playerBeginMatch[numPlayers] = { (char) 0x80, (char) 0x90 };
	int triangleText = 0x93860343;
	int triangleImage = 0x24060004;
	int playSound = 0x0C00FFB8;
	int drawGsCursor[numDrawCursorInst] = { 0x0C00DBE0, 0x0C01001B, 0x0C00DBE0 };
	int drawText = 0x0C00DB05;
	int duelistCodeRet = 0x24020001;
	int nop = 0x00000000;

	/* Data */
	char viewCard = 0x00;
	char blockCard = 0xFF;
}

const short PvPScene = 0x2D0;
const short duelScene = 0x2C3;
short currentScene;

char currentTurn;
char lastTurn = 0xFF;

bool p1Begin = false;
bool switchedSides = false;
bool duelEnded = true;
bool playing = false;

short lp[2];
int seed;

void HookMaxLPBypass()
{
	WriteMem(RAM::maxLP, &Cheat::nop, sizeof(Cheat::nop));
}

void AdjustLPString()
{
	for (int i = 0; i < numPlayers; i++)
	{
		ReadMem(RAM::lp[i] - 2, &lp[i], sizeof(lp[i]));
		if (lp[i] > 9999)
			WriteMem(RAM::maxLPDigits[i], &Cheat::maxLPDigits, sizeof(Cheat::maxLPDigits));
		else
			WriteMem(RAM::maxLPDigits[i], &Cheat::minLPDigits, sizeof(Cheat::minLPDigits));
	}
}

void HookDuelistCodeBypass()
{
	WriteMem(RAM::duelistCode, &Cheat::nop, sizeof(Cheat::nop));
	WriteMem(RAM::duelistCodeRet, &Cheat::duelistCodeRet, sizeof(Cheat::duelistCodeRet));
}

bool SceneIsPvP()
{
	ReadMem(RAM::currentScene, &currentScene, sizeof(currentScene));
	return currentScene == PvPScene;
}

bool SceneIsDuel()
{
	ReadMem(RAM::currentScene, &currentScene, sizeof(currentScene));
	return currentScene == duelScene;
}

bool TurnTransitioned()
{
	ReadMem(RAM::currentTurn, &currentTurn, sizeof(currentTurn));

	if (currentTurn != lastTurn)
	{
		lastTurn = currentTurn;
		return true;
	}
	return false;
}

void SetSeed()
{
	ReadMem(RAM::prng, &seed, sizeof(seed));
	cout << "\nSeed: 0x" << hex << seed << ". Make sure that your seed is synchronized with your opponent's starting seed." << endl;
}

void AdvanceSeed()
{
	for (int i = 0; i < 255; i++)
		seed = (seed * 0x41C64E6D) + 0x3039; // LCG using FM's params

	WriteMem(RAM::prng, &seed, sizeof(seed));
}

void RemoveTrapRotation()
{
	for (int i = 0; i < numTrapRotInst; ++i)
		WriteMem(RAM::trapRot[i], &Cheat::nop, sizeof(Cheat::nop));
}

void SwitchSides()
{
	if (SceneIsPvP())
	{
		if (!switchedSides)
		{
			switchedSides = true;
			duelEnded = false;
			AdvanceSeed();
			if (p1Begin)
			{
				WriteMem(RAM::playerBeginMatch, &Cheat::playerBeginMatch[1], sizeof(Cheat::playerBeginMatch[1]));
				p1Begin = false;
			}
			else
			{
				WriteMem(RAM::playerBeginMatch, &Cheat::playerBeginMatch[0], sizeof(Cheat::playerBeginMatch[0]));
				p1Begin = true;
			}
		}
	}
	else
	{
		switchedSides = false;
	}
}

void UpdateImgAndText(char player)
{
	if (player == currentTurn)
	{
		WriteMem(RAM::playGsSound, &Cheat::playSound, sizeof(Cheat::playSound));
		for (int i = 0; i < numDrawCursorInst; ++i)
			WriteMem(RAM::drawGsCursor[i], &Cheat::drawGsCursor[i], sizeof(Cheat::drawGsCursor[i]));
		WriteMem(RAM::drawGsSymbols, &Cheat::drawText, sizeof(Cheat::drawText));
		WriteMem(RAM::drawGsText, &Cheat::drawText, sizeof(Cheat::drawText));
		WriteMem(RAM::triangleImage, &Cheat::triangleImage, sizeof(Cheat::triangleImage));
		WriteMem(RAM::triangleText, &Cheat::triangleText, sizeof(Cheat::triangleText));
	}
	else
	{
		WriteMem(RAM::playGsSound, &Cheat::nop, sizeof(Cheat::nop));
		for (int i = 0; i < numDrawCursorInst; ++i)
			WriteMem(RAM::drawGsCursor[i], &Cheat::nop, sizeof(Cheat::nop));
		WriteMem(RAM::drawGsSymbols, &Cheat::nop, sizeof(Cheat::nop));
		WriteMem(RAM::drawGsText, &Cheat::nop, sizeof(Cheat::nop));
		WriteMem(RAM::triangleImage, &Cheat::nop, sizeof(Cheat::nop));
		WriteMem(RAM::triangleText, &Cheat::nop, sizeof(Cheat::nop));
	}
}

void ChangeCardView(char player)
{
	WriteMem(RAM::cardView[player], &Cheat::viewCard, sizeof(Cheat::viewCard));
	WriteMem(RAM::cardView[(player + 1) % 2], &Cheat::blockCard, sizeof(Cheat::blockCard));
}

bool LoadingLifePoints()
{
	bool loading = false;
	for (int i = 0; i < numPlayers; ++i)
	{
		ReadMem(RAM::lp[i], &lp[i], sizeof(lp[i]));
		if (!lp[i])
			loading = true;
	}
	return loading;
}

void CheckEndOfDuel()
{
	bool ended = false;
	for (int i = 0; i < numPlayers; ++i)
	{
		ReadMem(RAM::lp[i], &lp[i], sizeof(lp[i]));
		if (!lp[i])
			ended = true;
	}
	if (ended)
		UpdateImgAndText(currentTurn);
}

void HandleGame(char player)
{
	if (duelEnded)
	{
		SwitchSides();
	}
	else
	{
		if (SceneIsDuel())
		{
			ChangeCardView(player);
			if (!playing)
			{
				while (LoadingLifePoints())
					ChangeCardView(player);
				playing = true;
				TurnTransitioned();
				UpdateImgAndText(player);
			}

			AdjustLPString();
			if (TurnTransitioned())
				UpdateImgAndText(player);

			CheckEndOfDuel();
		}
		else
		{
			if (playing)
			{
				duelEnded = true;
				playing = false;
				UpdateImgAndText(currentTurn);
			}
		}
	}
}