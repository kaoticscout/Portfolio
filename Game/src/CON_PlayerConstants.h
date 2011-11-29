#ifndef PLAYER_CONSTANTS_H
#define PLAYER_CONSTANTS_H

#include "OgreString.h"
#include "OgrePrerequisites.h"

//Avatar Constants
const Ogre::String PLAYERMODELLOC = "Sinbad.mesh";
const int NUMITEMSLOTS = 6;
const int CHAR_HEIGHT = 5;
const int NUMINVENTORYSLOTS = 36;
const float PLAYER_BASE_MOVE_SPEED = 0.6f;
const float PLAYER_TURN_SPEED = 1.5f;

const enum ITEM_SLOT
{
	SLOT_HEAD = 0,
	SLOT_CHEST,
	SLOT_GLOVES,
	SLOT_BOOTS,
	SLOT_MH_WEAPON,
	SLOT_OH_WEAPON
};

const enum PLAYER_STATE
{
	PLAYER_IDLE = 0,
	PLAYER_MOVING,
	PLAYER_ATTACKING
};
#endif