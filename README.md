# FM-Online
FM Online is a collection of hacks for the original Yu-Gi-Oh! Forbidden Memories that fixes and enhances the online experience. It's intended to run along [mednafen](https://mednafen.github.io/) PSX emulator.

# Features
* Decks with the same duelist code can enter the PvP menu.
* Custom PRNG algorithm that fixes major desync issues with mednafen's online service.
* Automatic turn switch between each match along P1 and P2.
* Life points can go over 8000 when using healing magics.
* Major improvements in the opponent's POV, including:
    * Hidden cards.
    * Hidden guardian star selection.
    * Hidden card information on the board.
    * Hidden triangle preview.
    * Removal of automatic card rotation.

![Imgur](https://imgur.com/wlPx7Xo.jpg)

# Usage
This program runs alongside mednafen. Set up mednafen's emulator, connect online with your opponent inside the emulator and then run the FM Online executable before you start a duel. You can download the latest version of the program [here](https://github.com/mateusfavarin/FM-Online/releases). Once the software is running, make sure that the starting seed is in sync with your opponent's seed (the program will show the seed in the very first time you enter the PvP screen).

## Troubleshooting
In case there is a mismatch between the starting seeds, player 2 must:
* Close the executable.
* Close the online connection (`/quit` inside the emulator).
* Reconnect to the online server (`/connect` inside the emulator).
* Run the executable again.

Although rare, desyncs can still happen due to the mednafen's server connection. If that happens during a duel, a simple `/quit` followed by `/connect` from any player inside the emulator should fix the issue.

If any player closes the executable when playing a duel, then both players must close the executable, leave the duel and run the program again.

# Setting up the emulator

## Download
* [Mednafen](https://mednafen.github.io/)
* [MedGuiR](https://github.com/Speedvicio/MedGuiReborn/releases)

## BIOS
Mednafen expects valid PSX BIOS inside the `/firmware` folder. Dump your PSX BIOS there.

## MedGuiR
Restarting your `MedGuiR` instance may be required after some of the steps. Open `MedGuiR.exe`, and follow the steps:
* In the `General` tab, select your mednafen root folder.
* Click on the black command promp image on the top of the window to select your Yu-Gi-Oh! Forbidden Memories NTSC-U game. Mednafen expects you to select the `cue sheet` extension. In case you don't have that, you can generate it [here](https://www.duckstation.org/cue-maker/).
* Press CTRL + SHIFT in the MedGuiR window to open the sony playstation settings.
    * Net-Play tab:
        * Make sure that you and your opponent are in the same Net-Play Host.
        * Net-Play Local Player: 1
        * Set your nickname in the Net-Play Nickname field.
    * Playstation tab:
        * Do not select any multi tap.
    * Sound tab:
        * Set the sound buffer to 70.
    * Video tab:
        * You can set Window Scaling to whatever you feel most confortable. 1x1 = 320x240, do the math.
* Close the sony playstation window settings and save your configurations. Double click in the ROM name to open the game.
* Inside the emulator:
    * Press CTRL + SHIFT + 1 until it changes the controller 1 to Digital Gamepad. Repeat this step for the controllers 2, 3 and 4.
    * Press ALT + SHIFT + 1 to set your controller bindings. Do the same for the controller 2.
## Setting up the match
* You need two memory card files in order to play a match. Memory card files are in the `sav/` folder inside your mednafen's root folder. The slot 1 save file ends with `.0.mcr`, while the slot 2 save file ends with `.1.mcr`. To set the slot 1 file in the slot 2, close both your mednafen and MedGuiR instances, and then copy+rename the end of your slot 1 file.
* Inside the mednafen emulator, press `t` to open the chat box. Type `/connect` to connect to the server. A few other useful commands:
    * `/quit` - leaves the server.
    * `/drop #` - drops the # controller.
    * `/take #` - takes the # controller.
* Once both players are connected in the server, both players must run the FM Online executable before starting a duel. Make sure that the starting RNG matches for both players, as explained beforehand.