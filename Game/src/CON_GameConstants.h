#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

const int	FIRST_LEVEL_ID  = 1;
const float MAX_FRAME_TIME = 0.1f;

const enum GAME_STATE
{
	INITIALIZING_GAME,			//Load intial game components
	LOGO,						//Kaotic Productions logo screen
	MAIN_MENU,					//Login screen
	CHARACTER_SELECTION,		//Screen displaying all characters
	LOADING_LEVEL,				//Load resources for the current level
	PLAYING_LEVEL,				//Current Level is running
	ENDING_LEVEL,				//Clean up the previous level
	SAFE_STATE,					//Player in town, state for when the player is not playing the level but doing some other activity
	PAUSE_GAME,
	LOGGING_OUT,
	SHUTTING_DOWN
};

#endif