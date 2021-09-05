#include <filesystem>
#include <fstream>
#include <iostream>
#include "online.h"

using namespace std;
namespace fs = std::filesystem;

const string configFile = "config.ini";

string playerInput;
char player;

string gameInput;
string confirmationInput;

void GetPlayerInput()
{
	while (true)
	{
		cout << "Which player are you? [1/2]" << endl;
		getline(cin, playerInput);
		if (playerInput == "1" || playerInput == "2")
		{
			player = playerInput[0] - 1 - '0';
			cout << "Player: " << playerInput << " set.\n" << endl;
			return;
		}
		cout << "ERROR: Invalid input. Please type 1 if you're the player 1, or 2 if you're the player 2.\n" << endl;
	}
}

bool LoadConfigFile()
{
	for (const auto& entry : fs::directory_iterator("."))
	{
		if (entry.path().filename().string() == configFile)
		{
			ifstream file(configFile);
			getline(file, gameInput);
			file.close();
			cout << "Loaded ROM name: " << gameInput << " from config.ini" << endl;
			return true;
		}
	}
	return false;
}

void GetGameInput()
{
	while (true)
	{
		cout << "Please enter your ROM name (e.g: yugiohfm.bin):" << endl;
		getline(cin, gameInput);
		int dot = gameInput.find(".");

		if (dot != -1)
		{
			while (true)
			{
				cout << "\nPlease confirm that your ROM name is: [y/n]\nGame name: " << gameInput << endl;
				getline(cin, confirmationInput);
				if (confirmationInput == "y" || confirmationInput == "n")
					break;

				cout << "\nInvalid input. Type lowercase y for yes, or lowercase n for no.\n" << endl;
			}

			if (confirmationInput == "y")
			{
				gameInput = gameInput.substr(0, dot);
				while (true)
				{
					cout << "\nWould you like to save the ROM name to the configuration file? [y/n]" << endl;
					getline(cin, confirmationInput);
					if (confirmationInput == "y" || confirmationInput == "n")
						break;

					cout << "\nInvalid input. Type lowercase y for yes, or lowercase n for no.\n" << endl;
				}

				if (confirmationInput == "y")
				{
					ofstream file(configFile);
					file << gameInput;
					file.close();
				}

				cout << "ROM name: " << gameInput << " set." << endl;
				return;
			}

			cout << "\nOperation canceled. Please try again.\n" << endl;
		}
		else
		{
			cout << "\nInvalid input, file extension missing.\n" << endl;
		}
	}
}

int main(int argc, char * argv[])
{
	// since I don't use stdio, I can disable the sync which allows for faster iostream
	ios::sync_with_stdio(false);

	cout << "WARNING: you need to connect online via mednafen with your opponent in the FM PvP screen before running this program. Doing otherwise may desynchronize the deck shuffle.\n" << endl;

	GetPlayerInput();
	if (!LoadConfigFile())
		GetGameInput();

	if (!InitOnline(gameInput))
	{
		cout << "\nCouldn't find the emulator window. Make sure that your emulator is open, and that your ROM name matches the ROM name specified for this program." << endl;
		return 0;
	}
	
	cout << "\nFM Online connected successfully." << endl;

	Play(player);

	return 0;
}